#include <Adafruit_NeoPixel.h>
#define PIN        13
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void led_setup() {
  pixels.begin();
  pixels.show();
  pixels.setBrightness(255);
}

void red() {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
}

void green() {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();
}

void amber() {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(255, 96, 0));
  pixels.show();
}

void blue_wink() {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
  delay(200);
  green();
}
