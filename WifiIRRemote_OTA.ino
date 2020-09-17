#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
typedef ESP8266WebServer  WiFiWebServer;
#include <AutoConnect.h>
#include "secrets.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>
#include <Timer.h>
#include <DHT.h>
#include <FS.h>

#define PARAM_FILE      "/mqtt.json"


static const char CONFIG_PAGE[] PROGMEM = R"(
{
  "uri": "/mqtt",
  "title": "MQTT",
  "menu": true,
  "element": [
    {
      "name": "text",
      "type": "ACText",
      "value": "MQTT Configuration",
      "style": "font-family:Arial;font-size:18px;font-weight:400;color:#191970"
    },
    {
      "name": "mqttusername",
      "type": "ACInput",
      "label": "MQTT USERNAME",
      "placeholder":"Adafruit UserName",
      "pattern": "^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$"
    },
    {
      "name": "mqttpassword",
      "type": "ACInput",
      "label": "MQTT PASSWORD",
      "placeholder":"Adafruit AIO KEY",
      "pattern": "^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$"
    },
    {
      "name": "adafruitname",
      "type": "ACInput",
      "label": "Adafruit User Name",
      "placeholder":"Adafruit Username for feed configuration",
      "pattern": "^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$"
    },
    {
      "name": "Warning",
      "type": "ACText",
      "value": "Do Not Change the below Fields Unless you know What you are doing",
      "style": "font-family:Arial;font-size:18px;font-weight:400;color:#ba2d25"
    },
    {
      "name": "mqttserver",
      "type": "ACInput",
      "label": "MQTT Server",
      "placeholder":"MQTT Server",
      "value":"io.adafruit.com"
    },
    {
      "name": "mqttport",
      "type": "ACInput",
      "label": "MQTT Port",
      "placeholder":"MQTT Port",
      "value":"1883"
    },
    {
      "name": "irch",
      "type": "ACInput",
      "label": "IR Channel",
      "placeholder":"Channel To Receive IR Commands",
      "value":"/feeds/ircommands"
    },
    {
      "name": "commandch",
      "type": "ACInput",
      "label": "Device Control Channel",
      "placeholder":"Channel to Receive device Commands",
      "value":"/feeds/devicecommands"
    },
    {
      "name": "telemetrych",
      "type": "ACInput",
      "label": "Device Telemetry Channel",
      "placeholder":"Channel to Send Telenetry info",
      "value":"/feeds/devicetelemetry"
    },
    {
      "name": "statech",
      "type": "ACInput",
      "label": "Device State Channel",
      "placeholder":"Channel to Send Device State",
      "value":"/feeds/devicestate"
    },
    {
      "name": "load",
      "type": "ACSubmit",
      "value": "Load",
      "uri": "/mqtt"
    },
    {
      "name": "save",
      "type": "ACSubmit",
      "value": "Save",
      "uri": "/save"
    },
    {
      "name": "adjust_width",
      "type": "ACElement",
      "value": "<script type=\"text/javascript\">window.onload=function(){var t=document.querySelectorAll(\"input[type='text']\");for(i=0;i<t.length;i++){var e=t[i].getAttribute(\"placeholder\");e&&t[i].setAttribute(\"size\",40)}};</script>"
    }
  ]
}
)";

static const char SAVE_PAGE[] PROGMEM = R"(
{
  "uri": "/save",
  "title": "MQTT",
  "menu": false,
  "element": [
    {
      "name": "caption",
      "type": "ACText",
      "format": "Elements have been saved to %s",
      "style": "font-family:Arial;font-size:18px;font-weight:400;color:#191970"
    },
    {
      "name": "validated",
      "type": "ACText",
      "style": "color:red"
    },
    {
      "name": "echo",
      "type": "ACText",
      "style": "font-family:monospace;font-size:small;white-space:pre;"
    },
    {
      "name": "ok",
      "type": "ACSubmit",
      "value": "OK",
      "uri": "/mqtt"
    }
  ]
}
)";

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

void loadMQTTForm(){
    elementsAux.load(FPSTR(CONFIG_PAGE));
  elementsAux.on([] (AutoConnectAux& aux, PageArgument& arg) {
    if (portal.where() == "/mqtt") {
      SPIFFS.begin();
      File param = SPIFFS.open(PARAM_FILE, "r");
      if (param) {
        aux.loadElement(param, { "mqttusername", "mqttpassword", "adafruitname", "mqttserver", "mqttport", "irch","commandch","telemetrych","statech" });
        param.close();
      }
      SPIFFS.end();
    }
    return String();
  });
}

void saveMQTTForm(){
saveAux.load(FPSTR(SAVE_PAGE));
saveAux.on([] (AutoConnectAux& aux, PageArgument& arg) {
    AutoConnectInput& mqttusername = elementsAux["mqttusername"].as<AutoConnectInput>();
    aux["validated"].value = mqttusername.isValid() ? String() : String("Input data pattern missmatched.");
    aux["caption"].value = PARAM_FILE;

    SPIFFS.begin();
    File param = SPIFFS.open(PARAM_FILE, "w");
    if (param) {
      elementsAux.saveElement(param, { "mqttusername", "mqttpassword", "adafruitname", "mqttserver", "mqttport", "irch","commandch","telemetrych","statech" });
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
//    if (!client.connected()) {
//      amber();
//      long now = millis();
//      if (now - lastReconnectAttempt > 5000) {
//        lastReconnectAttempt = now;
//        if (reconnect()) {
//          lastReconnectAttempt = 0;
//        }
//      }
//    }
  }
  portal.handleClient();
}
