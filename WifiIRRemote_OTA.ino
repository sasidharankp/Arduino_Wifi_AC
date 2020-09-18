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

const char* temperatureTopic = TEMPERATURE_TOPIC;
const char* humidityTopic = HUMIDITY_TOPIC;

long lastReconnectAttempt = 0;
int previousHumidity = 0;
float previousTemperature = 0;
String nextState = "humidity";

void setup() {
  led_setup();
  red();
  randomSeed(micros());
  Serial.begin(115200);
  oledSetup();
  delay(500);

  server.on("/", []() {
    onRoot();
  });
  loadMQTTForm();
  saveMQTTForm();
  setMQTTDetails(elementsAux);

  config.ota = AC_OTA_BUILTIN;
  portal.join({ elementsAux, saveAux });
  config.ticker = true;
  portal.config(config);
  portal.begin();

  if (WiFi.status() == WL_CONNECTED) {
    amber();
    setup_mqtt();
  }
  dhtSetup();
  Serial.println("INITIAL TEMPERATURE: " + String(previousTemperature) + " INITIAL HUMIDITY: " + String(previousHumidity));
  t.every(60000, publishDhtData);
}

void loadMQTTForm() {
  elementsAux.load(FPSTR(CONFIG_PAGE));
  elementsAux.on([] (AutoConnectAux & aux, PageArgument & arg) {
    if (portal.where() == "/mqtt") {
      SPIFFS.begin();
      File param = SPIFFS.open(PARAM_FILE, "r");
      if (param) {
        aux.loadElement(param, { "mqttusername", "mqttpassword", "adafruitname", "mqttserver", "mqttport", "irch", "commandch", "telemetrych", "statech" });
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
      elementsAux.saveElement(param, { "mqttusername", "mqttpassword", "adafruitname", "mqttserver", "mqttport", "irch", "commandch", "telemetrych", "statech" });
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


void loop() {
  t.update();
  if (WiFi.status() != WL_CONNECTED)
  {
    red();
    Serial.println("WIFI DISCONNECTED");
  } else {
    if (!client.connected()) {
      long now = millis();
      if (now - lastReconnectAttempt > 5000) {
        lastReconnectAttempt = now;
        if (reconnect()) {
          lastReconnectAttempt = 0;
        }
      }
    } else {
      // Client connected
      client.loop();
    }
  }
  portal.handleClient();
}
