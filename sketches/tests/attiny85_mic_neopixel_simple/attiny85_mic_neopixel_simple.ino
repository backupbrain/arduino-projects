#include <Adafruit_NeoPixel.h>

/*
 * attiny85_adc_simple.cpp
 *
 * This example shows how to use ATtiny85 Analog-Digital conversion functionality
 */
 
const byte LED_PIN = 13;
const byte NEOPIXEL_PIN = 6;
const byte NUM_NEOPIXELS = 64;
const byte AUDIO_INPUT_PIN = A1;

const unsigned int sampleLength = 256;
 
unsigned int micReading = 0;
unsigned int sampleValues[sampleLength];
byte value = 0;
    
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_NEOPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    pinMode(AUDIO_INPUT_PIN, INPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
 
   unsigned int sampleIndex = 0;
  while(sampleIndex < sampleLength) {
    sampleValues[sampleIndex] = analogRead(AUDIO_INPUT_PIN); // get adc value
    sampleIndex++;
  }
  
  
    unsigned int intensity = 0; // peak-to-peak level
    
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;
       
    for (int i = 0; i < sampleLength; i++) {
      if (sampleValues[i] < 1024) {
        if (sampleValues[i] > signalMax) {
          signalMax = sampleValues[i]; // save just the max levels
        } else {
          signalMin = sampleValues[i]; // save just the min levels
        }
      } 
    }
    
    intensity = signalMax - signalMin; // max - min = peak-peak amplitude
    
    
  value = map(intensity, 0, 1024, 0, 255);
  analogWrite(LED_PIN, value);
 
   for (int i=strip.numPixels()-1; i > 0; i--) {
     strip.setPixelColor(i, strip.getPixelColor(i-1));
   }
   strip.setPixelColor(0, strip.Color(value, value, value));
   strip.show();
}
 
