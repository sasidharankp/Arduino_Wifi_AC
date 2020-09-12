#include <ESP8266WiFi.h>
char WLAN_SSID[]=WIFI_SSID;
char WLAN_PASS[]=WIFI_PASSWORD;



// Connect to WiFi access point.
boolean setup_wifi() {
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  amber();
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  return (WL_CONNECTED);
}
