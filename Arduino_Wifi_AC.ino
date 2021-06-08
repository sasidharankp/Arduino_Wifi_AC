#include <ESP8266WiFi.h>
#include "secrets.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>
#include <Timer.h>
#include <DHT.h>

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
  boolean wifi_status = setup_wifi();
  if (wifi_status) {
    setup_OTA();
    setup_mqtt();
  }
  dhtSetup();
  Serial.println("INITIAL TEMPERATURE: " + String(previousTemperature) + " INITIAL HUMIDITY: " + String(previousHumidity));
  t.every(60000, publishDhtData);
}

void loop() {
  t.update();
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
