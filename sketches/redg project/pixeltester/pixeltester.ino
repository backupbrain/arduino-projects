#include <Adafruit_NeoPixel.h>
#include "ColorConverter.h"
#include <MemoryFree.h>

const uint8_t ledPin = 13;

const uint16_t numPixelStrips = 5;
const uint16_t numPixelsPerStrip = 8;

const uint8_t neopixelPins[5] = {9, 10, 11, 12, 13};

const uint8_t percentBrightnessOffsets[5] = {
100, 
100, 
100, 
100, 
100
};


Adafruit_NeoPixel** strips;
HsvColor hsv;
RgbColor rgb;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  strips = (Adafruit_NeoPixel**)malloc(numPixelStrips*sizeof(Adafruit_NeoPixel*));
  
  
  
  for (uint8_t index = 0; index < numPixelStrips; index++) {
    strips[index] = new Adafruit_NeoPixel(numPixelsPerStrip, neopixelPins[index], NEO_GRB + NEO_KHZ800);
    strips[index]->begin();
  }
  
  digitalWrite(ledPin, HIGH);
  
}

void loop() {
  uint8_t percentBrightnessOffset = 100;
  uint8_t hue = 0;
  for (uint8_t stripIndex = 0; stripIndex < numPixelStrips; stripIndex++) {
    percentBrightnessOffset = percentBrightnessOffsets[stripIndex];
    
    
    hue = 360*((float)stripIndex/numPixelStrips);
    hsv.h = (unsigned char) floor(255*hue/360);
//    hsv.h = 30;
    hsv.s = 255;
    hsv.v = (255*percentBrightnessOffset)/100;
    
    rgb = ColorConverter::HsvToRgb(hsv);
    
    for (uint16_t pixelIndex = 0; pixelIndex < numPixelsPerStrip; pixelIndex++) {
      strips[stripIndex]->setPixelColor(pixelIndex, rgb.r, rgb.g, rgb.b);
    }
    strips[stripIndex]->show();
  }
  
  
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
}
