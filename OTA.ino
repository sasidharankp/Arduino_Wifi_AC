#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

AsyncWebServer server(80);

void setup_OTA(){  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  telemetry("Starting OTA Server");
    request->send(200, "text/plain", "Hi! I am ESP8266.");
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  telemetry("HTTP server started");
  AsyncElegantOTA.loop();
}
