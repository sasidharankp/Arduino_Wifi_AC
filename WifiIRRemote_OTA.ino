#if defined(ARDUINO_ARCH_ESP8266)
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
#endif

WiFiWebServer server;
AutoConnect portal(server);
AutoConnectConfig config;

const char* www_username = "admin";
const char* www_password = "esp8266";

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
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
    onRoot();
  });
  config.ota = AC_OTA_BUILTIN;
  portal.config(config);
  portal.begin();
  amber();
  if (WiFi.status() == WL_CONNECTED) {
    setup_mqtt();
  }
  dhtSetup();
  Serial.println("INITIAL TEMPERATURE: " + String(previousTemperature) + " INITIAL HUMIDITY: " + String(previousHumidity));
  t.every(60000, publishDhtData);
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
