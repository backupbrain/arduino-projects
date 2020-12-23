#include <Arduino.h>

void setup_bootsequence(Adafruit_NeoPixel* strip) {
  const unsigned long sequenceTimeout_ms = 250;
  
  for (int i=0; i<=strip->numPixels(); i++) {
    strip->setPixelColor(i, 255, 0, 0);
  }
  strip->show();

  delay(sequenceTimeout_ms);
  for (int i=0; i<=strip->numPixels(); i++) {
    strip->setPixelColor(i, 0, 255, 0);
  }
  strip->show();

  
  delay(sequenceTimeout_ms);
  for (int i=0; i<=strip->numPixels(); i++) {
    strip->setPixelColor(i, 0, 0, 255);
  }
  strip->show();

  delay(sequenceTimeout_ms);
  
}

