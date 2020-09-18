void setMQTTDetails(AutoConnectAux& aux) {
  SPIFFS.begin();
  File param = SPIFFS.open(PARAM_FILE, "r");
  if (param) {
    if (aux.loadElement(param)) {
      aux.loadElement(param, { "mqttusername", "mqttpassword", "adafruitname", "mqttserver", "mqttport", "irch", "commandch", "telemetrych", "statech" });
      MQTT_SERVER = (aux["mqttserver"].value);
      MQTT_USERNAME = (aux["mqttusername"].value);
      MQTT_PORT = (aux["mqttport"].value).toInt();
      MQTT_PASSWORD = (aux["mqttpassword"].value);
      FEED_USERNAME = aux["adafruitname"].value;
      IR_COMMANDS = (aux["irch"].value);
      DEVICE_STATE = (aux["statech"].value);
      DEVICE_TELEMETRY = (aux["telemetrych"].value);
      param.close();
    }
    SPIFFS.end();
  }
}

void setup_mqtt() {
  client.setServer(MQTT_SERVER.c_str(), MQTT_PORT);
  client.setCallback(decodeMessage);
  int connectCount = 1;
  while (!client.connected() && connectCount < 6) {
    amber();
    connectCount++;
    Serial.println("MQTT Initial Connect Attempt: " + String(connectCount));
    if (client.connect(clientId.c_str(), MQTT_USERNAME.c_str(), MQTT_PASSWORD.c_str() )) {
      green();
      deviceInfo = ("IP Address: " + (WiFi.localIP()).toString() + " MAC Address: " + String(WiFi.macAddress()) + " connected as " + clientId);
      telemetry(deviceInfo);
      if (client.subscribe((MQTT_USERNAME + IR_COMMANDS).c_str())) {
        telemetry("Subscribed to " + (MQTT_USERNAME + IR_COMMANDS));
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
  if (client.connect(clientId.c_str(), MQTT_USERNAME.c_str(), MQTT_PASSWORD.c_str() )) {
    green();
    telemetry("Reconnected");
    if (!client.subscribe((MQTT_USERNAME + IR_COMMANDS).c_str())) {
      client.subscribe((MQTT_USERNAME + IR_COMMANDS).c_str());
    }
    if (client.subscribe((MQTT_USERNAME + IR_COMMANDS).c_str())) {
      telemetry("Subscribed to " + String((MQTT_USERNAME + IR_COMMANDS)));
    }
  }
  return client.connected();
}
