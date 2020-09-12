void dhtSetup()
{
  dht.begin();
  delay(100);
  previousHumidity = dht.readHumidity();
  previousTemperature = dht.readTemperature();
}

void publishDhtData() {
  if (nextState == "humidity") {
    publishHumidity();
    nextState = "temperature";
  } else if (nextState == "temperature") {
    publishTemperature();
    nextState = "humidity";
  }
}

void publishHumidity() {
  int currentHumidity = dht.readHumidity();
  if (isnan(currentHumidity)) {
    telemetry("Failed to read Humidity from sensor!");
    return;
  } else if (currentHumidity < 100) {
    telemetry("Previous Humidity: " + String(previousHumidity) + " Current Humidity " + String(currentHumidity));
    if (previousHumidity != currentHumidity) {
      client.publish(humidityTopic, String(currentHumidity).c_str(), true);
      previousHumidity = currentHumidity;
    }
    showHumidity(String(currentHumidity));
  }
}
void publishTemperature() {
  float currentTemperature = dht.readTemperature();
  if (isnan(currentTemperature)) {
    telemetry("Failed to read Humidity from sensor!");
    return;
  } else if (currentTemperature < 100) {
    telemetry("Previous Temperature: " + String(previousTemperature) + " Current Temperature " + String(currentTemperature));
    if (previousTemperature != currentTemperature) {
      client.publish(temperatureTopic, String(currentTemperature).c_str(), true);
      previousTemperature = currentTemperature;
    }
    showTemperature(String(currentTemperature));
  }
}
