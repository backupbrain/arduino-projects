#include <avr/pgmspace.h>
#include "fireplace.h"
#include "SimpleNeoPixel.h"
//#include <MemoryFree.h>


/** 
 * Define chips and pinouts
 */
#define __BOARD__ ATTINY
//#define __BOARD__ ARDUINO

#if (__BOARD__ == ATTINY)
#define PWM_PIN 0 // 0 = pin 0 on ATTiny
#else
#define PWM_PIN 5 // 5 = pin 9 on blend micro
#endif

/**
 * LED strip configuration
 */
#define NUM_PIXELS 144

// each pixel consumes 2*(1+1+2) = 8 bytes of data
// the ATTiny has 512 bytes of RAM.  MAX: ~ (512/8)= 64 pixels
// the Arduino has 2048 bytes of ram.  MAX: ~ (2048/8) = 256 pixels
#define DIVIDER 9 // how many times we divide the led strip to conserve memory
const byte numWorkingPixels = NUM_PIXELS/DIVIDER;

const byte NEOPIXEL_PIN = PWM_PIN;

const unsigned int FRAMERATE_MS = 16; //16; // 16 ms = 60 fps

/** 
 * Our pre-programmed pixel colors
 */
const byte numPixelColors = 64;

const unsigned long pixels[] PROGMEM = {
  0x7C70634,
  0xAE132A0,
  0x18FB479F,
  0x17F70834,
  0xFCF369F,
  0x17E6349F,
  0xDDE3F34,
  0xD31C34,
  0xCF23A9F,
  0x18D028A0,
  0xFDD3F34,
  0xECE0B34,
  0x17E839A0,
  0x7E20134,
  0xCDE329F,
  0x6F91334,
  0x15C21534,
  0xCE52E34,
  0x10F22234,
  0xECD2A34,
  0x16E034A0,
  0x18F4309F,
  0x1CC0C9F,
  0x8D8009F,
  0x8E6389F,
  0xAC53234,
  0x5F43634,
  0x13C81D9F,
  0xAD23C34,
  0x7D029A0,
  0x1C510A0,
  0x16DB1934,
  0x10E814A0,
  0x15E737A0,
  0x2DA3F9F,
  0x1CA119F,
  0x3F617A0,
  0x15FB43A0,
  0x8C42134,
  0x11FA039F,
  0x11C53DA0,
  0x1DF1434,
  0x18EA3734,
  0x12E74434,
  0xDFC109F,
  0x2F42DA0,
  0xECA1E34,
  0x11D6079F,
  0xEE40D9F,
  0x3D742A0,
  0xFF34534,
  0x8FA1F9F,
  0xFDF2B34,
  0x10C13F9F,
  0xFDF3134,
  0x16E91234,
  0x14BE309F,
  0x2E62C9F,
  0x5F82EA0,
  0x7C3379F,
  0x14EF0A9F,
  0x17E72FA0,
  0x7D423A0,
  0x7E04634,
};



/**
 * Runtime stuff
 */
PixelPointer pixelPointers[NUM_PIXELS];
rgb rgbPixels[numWorkingPixels];

SimpleNeopixel strip = SimpleNeopixel(NUM_PIXELS, NEOPIXEL_PIN);

unsigned long frameStartTime_ms = 0;
unsigned long currentTime = 0;


void setup() {
    //Serial.begin(9600);
    
    initializeFireplace();
    
    frameStartTime_ms = millis();
}

void loop() {
  currentTime = millis();
  int timeDifference = currentTime - frameStartTime_ms;
  if (timeDifference > FRAMERATE_MS) {
    updateFireplace();
    displayFireplace();
    frameStartTime_ms = currentTime;
  }
  
  //Serial.println(2560-freeMemory());
}
/* */



/**
 * Initialize the pixel values for the led strip
 */
void initializeFireplace() {
    // set up pixels
    for (unsigned int index = 0; index < numWorkingPixels; index++) {
      pixelPointers[index].fromPixelIndex = rand() % numPixelColors;
      pixelPointers[index].toPixelIndex = rand() % numPixelColors;
      
      Pixel p;
      p = readPixelPackage(pixelPointers[index].toPixelIndex, pixels);
      pixelPointers[index].timeout = p.timeout;
    }
    
    strip.begin();
}

/**
 * Update the pixel values for the led strip by tweening colors and resetting expired ones
 */
void updateFireplace() {
  int timeDifference = currentTime - frameStartTime_ms;
  
  // go through each pixel
  // push towards the timeout with the tween algorithm
  // if the timeout is expired, reset the pixel
  
  for (unsigned int index = 0; index < numWorkingPixels; index++) {
    if (pixelPointers[index].timeout <= 0) {
      pixelPointers[index].fromPixelIndex = pixelPointers[index].toPixelIndex;
      
      pixelPointers[index].toPixelIndex = rand() % numPixelColors;
      Pixel p;
      p = readPixelPackage(pixelPointers[index].toPixelIndex, pixels);
      pixelPointers[index].timeout = p.timeout;
      
      
    }
    
    // subtract the time difference
    if (timeDifference > pixelPointers[index].timeout) {
      pixelPointers[index].timeout = 0;
    } else {
      pixelPointers[index].timeout = pixelPointers[index].timeout - timeDifference;
    }
    
    
    // tween between pixels
    Pixel startPixel = readPixelPackage(pixelPointers[index].fromPixelIndex, pixels);
    Pixel endPixel = readPixelPackage(pixelPointers[index].toPixelIndex, pixels);
    
    hsv currentPixelColor = tweenPixel(startPixel, endPixel, pixelPointers[index].timeout);
    rgbPixels[index] = ColorConverter::hsv2rgb(currentPixelColor);
    
    
  }
  
  
}
/* */

/**
 * Push updated colors onto the led strip
 */
void displayFireplace() {
  unsigned int multiplier = DIVIDER-1;
  unsigned int index;
  cli();  
  do {
    index = numWorkingPixels-1;
    do {
      strip.sendPixel(rgbPixels[index].r, rgbPixels[index].g, rgbPixels[index].b);
    } while (index--);
  } while (multiplier--);
  sei();
  strip.show();
  
}
/* */

  

hsv tweenPixel(Pixel startPixel, Pixel endPixel, unsigned int timeout) {
  hsv currentColor;
  
  float percentDone = 1-((float)timeout/(endPixel.timeout));
  
  float h = ((endPixel.color.h - startPixel.color.h)*percentDone + startPixel.color.h);
  float s = ((endPixel.color.s - startPixel.color.s)*percentDone + startPixel.color.s);
  float v = ((endPixel.color.v - startPixel.color.v)*percentDone + startPixel.color.v);
  
  currentColor.h = h;
  currentColor.s = s;
  currentColor.v = v;
  
  return currentColor;
}

Pixel readPixelPackage(unsigned int pixelNumber, const long unsigned int* memoryAddress) {
  unsigned long package = pgm_read_dword_near(memoryAddress+pixelNumber);
  Pixel pixel = unpackPixel(package);
  
  return pixel;
}


Pixel unpackPixel(unsigned long package) {
  Pixel pixel;
  
  pixel.timeout = (package & 0xFF) * 8;
  pixel.color.v = (package >> 8) & 0xFF;
  pixel.color.s = (package >> 16) & 0xFF;
  pixel.color.h = (package >> 24) & 0xFF;
  
  return pixel;
}
