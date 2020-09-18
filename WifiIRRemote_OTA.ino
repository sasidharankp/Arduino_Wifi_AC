#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
typedef ESP8266WebServer  WiFiWebServer;
#include <AutoConnect.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>
#include <Timer.h>
#include <DHT.h>
#include <FS.h>
#include "pages.h"

#define PARAM_FILE      "/mqtt.json"




WiFiWebServer server;
AutoConnect portal(server);
AutoConnectConfig config;
AutoConnectAux  elementsAux;
AutoConnectAux  saveAux;

Timer t;

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

long lastReconnectAttempt = 0;
int previousHumidity = 0;
float previousTemperature = 0;
String nextState = "humidity";



void loadMQTTForm() {
  elementsAux.load(FPSTR(CONFIG_PAGE));
  elementsAux.on([] (AutoConnectAux & aux, PageArgument & arg) {
    if (portal.where() == "/mqtt") {
      SPIFFS.begin();
      File param = SPIFFS.open(PARAM_FILE, "r");
      if (param) {
        aux.loadElement(param, { "mqttusername", "mqttpassword", "adafruitname", "mqttserver", "mqttport", "irch", "telemetrych", "statech" });
        param.close();
      }
      SPIFFS.end();
    }
    return String();
  });
}

void saveMQTTForm() {
  saveAux.load(FPSTR(SAVE_PAGE));
  saveAux.on([] (AutoConnectAux & aux, PageArgument & arg) {
    AutoConnectInput& mqttusername = elementsAux["mqttusername"].as<AutoConnectInput>();
    aux["validated"].value = mqttusername.isValid() ? String() : String("Input data pattern missmatched.");
    aux["caption"].value = PARAM_FILE;

    SPIFFS.begin();
    File param = SPIFFS.open(PARAM_FILE, "w");
    if (param) {
      elementsAux.saveElement(param, { "mqttusername", "mqttpassword", "adafruitname", "mqttserver", "mqttport", "irch", "telemetrych", "statech" });
      param.close();
      // Read the saved elements again to display.
      param = SPIFFS.open(PARAM_FILE, "r");
      aux["echo"].value = param.readString();
      param.close();
    }
    else {
      aux["echo"].value = "SPIFFS failed to open.";
    }
    SPIFFS.end();
    return String();
  });
}



void onRoot() {
  WiFiWebServer& server = portal.host();
  server.sendHeader("Location", String("http://") + server.client().localIP().toString() + "/smartAC");
  server.send(302, "text/plain", "");
  server.client().flush();
  server.client().stop();
}



String deviceInfo = "";
String MQTT_SERVER = "";
String MQTT_USERNAME = "";
unsigned int MQTT_PORT = 1883;
String MQTT_PASSWORD = "";
String FEED_USERNAME = ""; //some mqtt brokers require account username
String IR_COMMANDS = "";
String DEVICE_STATE = "";
String DEVICE_TELEMETRY = "";
String clientId = ("ESP8266AcRemote" + String(random(0xfffff), HEX));
