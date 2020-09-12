#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void oledSetup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(500);
  display.clearDisplay();
}

void showTemperature(String temperature) {
  display.clearDisplay();
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 11);
  display.println("Temperature");
  display.setFont(&FreeMono12pt7b);
  display.setTextSize(3);
  display.setCursor(0, 59);
  display.println(temperature.substring(0,2));
  display.setFont(&FreeMono9pt7b);
  display.setTextSize(2);
  display.setCursor(80, 59);
  display.println(temperature.substring(3,4));
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(2);
  display.setCursor(104, 59);
  display.println(F("C"));
  display.display();
}

void showHumidity(String humidity) {
  display.clearDisplay();
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 11);
  display.println("Humidity");
  display.setFont(&FreeMono12pt7b);
  display.setTextSize(3);
  display.setCursor(0, 59);
  display.println(humidity.substring(0,2));
  display.setFont(&FreeMono9pt7b);
  display.setTextSize(2);
  display.setCursor(80, 59);
  display.println(humidity.substring(3,4));
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(2);
  display.setCursor(104, 59);
  display.println(F("%"));
  display.display();
}

//void showTemperature(String temperature) {
//  display.setFont(&FreeSans9pt7b);
//  display.clearDisplay();
//  display.setTextSize(5);
//  display.setTextColor(SSD1306_WHITE);
//  display.setCursor(0, 59);
//  display.println(temperature.substring(0,2));
//  display.setCursor(104, 25);
//  display.setTextSize(2);
//  display.println(temperature.substring(3,4));
//  display.setCursor(104, 59);
//  display.println(F("C"));
//  display.display();
//}
//void showHumidity(String humidity) {
//  display.setFont(&FreeSans9pt7b);
//  display.clearDisplay();
//  display.setTextSize(5);
//  display.setTextColor(SSD1306_WHITE);
//  display.setCursor(0, 59);
//  display.println(humidity.substring(0,2));
//  display.setCursor(104, 25);
//  display.setTextSize(2);
//  display.println(humidity.substring(3,4));
//  display.setCursor(104, 40);
//  display.setFont();
//  display.setTextSize(4);
//  display.println(F("%"));
//  display.display();
//  delay(4500);
//}
