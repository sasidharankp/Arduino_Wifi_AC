StaticJsonDocument<500> doc;

void decodeMessage(char* topic, byte* payload, unsigned int length) {
  Serial.println();
  telemetry("Message arrived in topic: " + String(topic));
  deserializeJson(doc, payload, 500);
  JsonObject object = doc.as<JsonObject>();

  if (object.isNull()) {
    telemetry("Invalid Json Message");
  } else {
    blue_wink();
    boolean power = doc["power"];
    uint8_t mode = doc["mode"];
    boolean celsius = doc["celsius"];
    short degrees = doc["degrees"];
    uint8_t fanspeed = doc["fanspeed"];
    const char* swingv = doc["swingv"];
    boolean light = doc["light"];
    boolean beep = doc["beep"];
    boolean econo = doc["econo"];
    boolean filter = doc["filter"];
    boolean turbo = doc["turbo"];
    boolean quiet = doc["quiet"];
    boolean clean = doc["clean"];
    int protocol = doc["protocol"];
    String manufacturer = doc["manufacturer"];

    IrSend(power, mode, celsius, degrees, fanspeed, swingv, light, beep, econo, filter, turbo, quiet, clean);
    Serial.println("-----------------------");
    Serial.print("power: ");
    Serial.println(power);
    Serial.print("mode: ");
    Serial.println(mode);
    Serial.print("celsius: ");
    Serial.println(celsius);
    Serial.print("degrees: ");
    Serial.println(degrees);
    Serial.print("fanspeed: ");
    Serial.println(fanspeed);
    Serial.print("swingv: ");
    Serial.println(swingv);
    Serial.print("light: ");
    Serial.println(light);
    Serial.print("beep: ");
    Serial.println(beep);
    Serial.print("econo: ");
    Serial.println(econo);
    Serial.print("filter: ");
    Serial.println(filter);
    Serial.print("turbo: ");
    Serial.println(turbo);
    Serial.print("quiet: ");
    Serial.println(quiet);
    Serial.print("clean: ");
    Serial.println(clean);
    Serial.print("protocol: ");
    Serial.println(protocol);
    Serial.print("manufacturer: ");
    Serial.println(manufacturer);
    Serial.println("-----------------------");
  }
}
