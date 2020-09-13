const char* mqttServer = MQTT_SERVER;
const int mqttPort = MQTT_PORT;
const char* mqttUser = MQTT_USERNAME;
const char* mqttPassword = MQTT_PASSWORD;
const char* listeningTopic = LISTENING_TOPIC;

String deviceInfo="";
void setup_mqtt() {

  client.setServer(mqttServer, mqttPort);
  client.setCallback(decodeMessage);
  while (!client.connected()) {
    amber();
    Serial.println("Connecting to MQTT...");
    String clientId = "ESP8266AcRemote";
    clientId += String(random(0xfffff), HEX);
    if (client.connect("clientId", mqttUser, mqttPassword )) {
      green();
      deviceInfo=("IP Address: "+(WiFi.localIP()).toString()+" MAC Address: "+String(WiFi.macAddress())+" connected as " + clientId);
      telemetry(deviceInfo);
      if (client.subscribe(listeningTopic)) {
        telemetry("Subscribed to " + String(listeningTopic));
      }
    } else {
      Serial.println("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

boolean reconnect() {
  amber();
  if (client.connect("clientId", mqttUser, mqttPassword )) {
    green();
    telemetry("Reconnected");
    if (!client.subscribe(listeningTopic)) {
      client.subscribe(listeningTopic);
    }
    if (client.subscribe(listeningTopic)) {
      telemetry("Subscribed to " + String(listeningTopic));
    }
  }
  return client.connected();
}
