#include <avr/pgmspace.h>
#include "FireplaceBacklight.h"
#include "SimpleNeoPixel.h"
//#include <MemoryFree.h>



#define __BOARD__ ATTINY
//#define __BOARD__ ARDUINO

#if (__BOARD__ == ATTINY)
#define PWM_PIN 0 // 0 = pin 0 on ATTiny
#else
#define PWM_PIN 5 // 5 = pin 9 on blend micro
#endif


/**
 *  Neopixel Configuration
 */
// each pixel consumes 2*(1+1+2) = 8 bytes of data
// the ATTiny has 512 bytes of RAM.  MAX: ~ (512/8)= 64 pixels
// the Arduino has 2048 bytes of ram.  MAX: ~ (2048/8) = 256 pixels
const unsigned int LED_COUNT = 144; // number of LED pins, 144
const byte NEOPIXEL_PIN = PWM_PIN; // PWM-compatible output pin


const unsigned int FRAMERATE_MS = 16; //16; // 16 ms = 60 fps


const byte numPixelColors = 48;
const unsigned long pixels[] PROGMEM = {
  0x7C70634,
  0xE132A0,
  0x18FB471D,
  0x17F70834,
  0x14CF361D,
  0x1CE6341D,
  0x8DE3F34,
  0xAD31C34,
  0x1BF23A1D,
  0x1DD028A0,
  0xFDD3F34,
  0x13CE0B34,
  0x21E839A0,
  0x7E20134,
  0x11DE321D,
  0x1F91334,
  0x15C21534,
  0x16E52E34,
  0x1AF22234,
  0x9CD2A34,
  0xCE034A0,
  0x18F4301D,
  0x10CC0C1D,
  0x17D8001D,
  0xDE6381D,
  0xAC53234,
  0x5F43634,
  0x1DC81D1D,
  0x1ED23C34,
  0x20D029A0,
  0x15C510A0,
  0xCDB1934,
  0x1FE814A0,
  0x15E737A0,
  0x11DA3F1D,
  0x1CA111D,
  0x12F617A0,
  0x10FB43A0,
  0x12C42134,
  0x1BFA031D,
  0x20C53DA0,
  0x1FDF1434,
  0x4EA3734,
  0x21E74434,
  0x21FC101D,
  0x20F42DA0,
  0x9CA1E34,
  0x7D6071D
};



FireplaceBacklight fireplaceBacklight = FireplaceBacklight(NEOPIXEL_PIN, LED_COUNT, pixels, numPixelColors);

unsigned long frameStartTime_ms = 0;
unsigned long currentTime = 0;

void setup() {
    frameStartTime_ms = millis();
}
void loop() {
  
  currentTime = millis();
  int timeDifference = currentTime - frameStartTime_ms;
  if (timeDifference > FRAMERATE_MS) {
    fireplaceBacklight.updateFrame();
    fireplaceBacklight.displayFrame();
    frameStartTime_ms = currentTime;
  }
  
}

