//const char* telemetryTopic= "sasidharankp/feeds/telemetry";
void telemetry(String message) {
  Serial.println(message);
//  client.publish(telemetryTopic ,String(message).c_str());
}
