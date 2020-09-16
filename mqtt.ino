//const char* mqttServer = MQTT_SERVER;
//const int mqttPort = MQTT_PORT;
//const char* mqttUser = MQTT_USERNAME;
//const char* mqttPassword = MQTT_PASSWORD;
//const char* irch = LISTENING_TOPIC;

String deviceInfo="";
void setup_mqtt(char * mqttserver,uint16_t mqttport,char * mqttusername,char * mqttpassword, char * clientID ) {
  Serial.println(" IP Address: "+(WiFi.localIP()).toString());
  client.setServer(mqttserver, mqttport);
  client.setCallback(decodeMessage);
  while (!client.connected()) {
    amber();
    Serial.println("Connecting to MQTT...");
    Serial.println(clientID);
    Serial.println(mqttusername);
    Serial.println(mqttpassword);
    if (client.connect(clientId, mqttusername, mqttpassword )) {
      green();
      deviceInfo=("IP Address: "+(WiFi.localIP()).toString()+" MAC Address: "+String(WiFi.macAddress())+" connected as " + clientId);
      telemetry(deviceInfo);
      if (client.subscribe(IR_COMMANDS)) {
        telemetry("Subscribed to " + String(IR_COMMANDS));
      }
    } else {
      Serial.println("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

//boolean reconnect() {
//  amber();
//  if (client.connect(clientId, mqttusername, mqttpassword )) {
//    green();
//    telemetry("Reconnected");
//    if (!client.subscribe(IR_COMMANDS)) {
//      client.subscribe(IR_COMMANDS);
//    }
//    if (client.subscribe(IR_COMMANDS)) {
//      telemetry("Subscribed to " + String(IR_COMMANDS));
//    }
//  }
//  return client.connected();
//}
