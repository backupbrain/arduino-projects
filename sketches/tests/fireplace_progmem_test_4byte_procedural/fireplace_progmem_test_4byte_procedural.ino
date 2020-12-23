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
#define NEOPIXEL_PIN_BIT 0 // 0 = pin 0 on ATTiny
#else
#define NEOPIXEL_PIN_BIT 5 // 5 = pin 9 on blend micro
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

#define FRAMERATE_MS 16 //16; // 16 ms = 60 fps

/** 
 * Our pre-programmed pixel colors
 */
#define numPixelColors 48;
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


/**
 * Runtime stuff
 */
PixelPointer pixelPointers[NUM_PIXELS];
rgb rgbPixels[numWorkingPixels];


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
 * FIREPLACE CODE
 */

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
    
    neopixel_begin();
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
    rgbPixels[index] = hsv2rgb(currentPixelColor);
    
    
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
      neopixel_sendPixel(rgbPixels[index].r, rgbPixels[index].g, rgbPixels[index].b);
    } while (index--);
  } while (multiplier--);
  sei();
  neopixel_show();
  
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

