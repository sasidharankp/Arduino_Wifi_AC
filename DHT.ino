void dhtSetup()
{
  dht.begin();
  delay(100);
  previousHumidity = dht.readHumidity();
  previousTemperature = dht.readTemperature();
}

const size_t dhtJsonCapacity = JSON_OBJECT_SIZE(4);
DynamicJsonDocument dhtJson(dhtJsonCapacity);

void readDhtData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    publishTemperatureJson(0,0,false);
  }else{
    publishTemperatureJson(t,h,true);
  }
  }

  void publishTemperatureJson(float temperature, float humidity, bool isSensorAvailable){
  dhtJson["isSensorAvailable"] = isSensorAvailable;
  dhtJson["sensor_type"] = String(DHTTYPE);
  dhtJson["temperature"] = temperature;
  dhtJson["humidity"] = humidity;
  
  char buffer[256];
  size_t n = serializeJson(dhtJson, buffer);
  client.publish((MQTT_USERNAME + DEVICE_STATE).c_str(),buffer, n);
  }

                          
