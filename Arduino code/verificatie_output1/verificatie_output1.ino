#include <Adafruit_NeoPixel.h>

#define PIN        6     
#define NUMPIXELS  5      

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();

  pixels.setPixelColor(0, pixels.Color(255, 255, 0));

  pixels.setPixelColor(1, pixels.Color(0, 0, 255));

  pixels.setPixelColor(2, pixels.Color(0, 255, 0));

  pixels.setPixelColor(3, pixels.Color(255, 0, 0));

  pixels.setPixelColor(4, pixels.Color(255, 165, 0));

  pixels.show();
}

void loop() {
}
