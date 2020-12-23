/*
  FireplaceBacklight.cpp - Library for powering neopixel fireplace backlight
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.
*/
#include "FireplaceBacklight.h"

/**
 * Create a FireplaceBacklight
 *
 * @param neopixelPin the output PWM pin that communicates with the NeoPixels
 * @param numNeopixels the number of NeoPixels we are powering
 * @param pixelMemoryAddress the EEPROM pixel memory address
 * @param numPixelColors the total number of preset colors in EEPROM
 */
FireplaceBacklight::FireplaceBacklight(unsigned char neopixelPin, unsigned int numNeopixels, const unsigned long* pixelMemoryAddress, unsigned int numPixelColors) {
  this->_neopixelPin = neopixelPin;
  this->_pixelMemoryAddress = pixelMemoryAddress;
  this->_numPixelColors = numPixelColors;
  
  this->_DIVIDER = 18;
  
  this->_numWorkingPixels = numNeopixels/this->_DIVIDER;
  
  // initialize _pixels and _oldPixels  
  this->_pixelPointers = (PixelPointer*) malloc (this->_numWorkingPixels*sizeof(PixelPointer));
  this->_rgbColors = (rgb*) malloc (this->_numWorkingPixels*sizeof(rgb));

  // make room for the neopixels
  this->_pixelStrip = (SimpleNeopixel*) malloc (sizeof(SimpleNeopixel*));
        
  this->_pixelStrip = new SimpleNeopixel(numNeopixels,  this->_neopixelPin);
  this->_pixelStrip->begin();
  
  this->_initialize();
}


FireplaceBacklight::~FireplaceBacklight() {
  free(this->_pixelStrip);
  this->_pixelStrip = NULL;
  free(this->_pixelPointers);
  this->_pixelPointers = NULL;
  free(this->_rgbColors);
  this->_rgbColors = NULL;
}


/**
 * Set up the initial values for the pixels
 */
void FireplaceBacklight::_initialize() {
  
  this->_lastFrameUpdate_ms = 0;
  this->_currentFrameUpdate_ms = millis();
  
  for (unsigned int index = 0; index < this->_numWorkingPixels; index++) {    
    this->_pixelPointers[index].fromPixelIndex = rand() % this->_numPixelColors;
    this->_pixelPointers[index].toPixelIndex = rand() % this->_numPixelColors;
      
    Pixel p;
    p = this->_readPixelFromEEPROM(this->_pixelPointers[index].toPixelIndex, this->_pixelMemoryAddress);
    this->_pixelPointers[index].timeout = p.timeout;
  }
  
}

/**
 * Read memory address from EEPROM and unpack into a Pixel
 */
Pixel FireplaceBacklight::_readPixelFromEEPROM(unsigned int pixelNumber, const long unsigned int* memoryAddress) {
  unsigned long package = pgm_read_dword_near(memoryAddress+pixelNumber);
  Pixel pixel = this->_unpackPixel(package);
  return pixel;
}

/**
 * unpack binary encoded data into a Pixel struct
 * 
 * @param package a binary encoded long containing Pixel data
 * @return a Pixel struct
 */
Pixel FireplaceBacklight::_unpackPixel(unsigned long package) {
  Pixel pixel;
  
  pixel.timeout = (package & 0xFF) * 8;
  pixel.color.v = (package >> 8) & 0xFF;
  pixel.color.s = (package >> 16) & 0xFF;
  pixel.color.h = (package >> 24) & 0xFF;
  
  return pixel;
}

/**
 * Update the animation frame.
 * This will change the light colors based on the current timestamp
 * 
 * @param timeDifference the difference since the last frame update
 */
void FireplaceBacklight::updateFrame() { 
  
  this->_lastFrameUpdate_ms = this->_currentFrameUpdate_ms;
  this->_currentFrameUpdate_ms = millis();
  
  unsigned int timeDifference = this->_currentFrameUpdate_ms - this->_lastFrameUpdate_ms;
  /* */
  
  for (unsigned int index = 0; index < this->_numWorkingPixels; index++) {
    // if this color change is over, pick a new color and begin tweening again
    if (this->_pixelPointers[index].timeout <= 0) {
      this->_pixelPointers[index].fromPixelIndex = this->_pixelPointers[index].toPixelIndex;
      
      this->_pixelPointers[index].toPixelIndex = rand() % this->_numPixelColors;
      Pixel p;
      p = this->_readPixelFromEEPROM(this->_pixelPointers[index].toPixelIndex, this->_pixelMemoryAddress);
      this->_pixelPointers[index].timeout = p.timeout;
    }
    
    // subtract the time difference
    if (timeDifference > this->_pixelPointers[index].timeout) {
      this->_pixelPointers[index].timeout = 0;
    } else {
      this->_pixelPointers[index].timeout = this->_pixelPointers[index].timeout - timeDifference;
    }
    
    
    // tween between pixels
    Pixel startPixel = this->_readPixelFromEEPROM(this->_pixelPointers[index].fromPixelIndex, this->_pixelMemoryAddress);
    Pixel endPixel = this->_readPixelFromEEPROM(this->_pixelPointers[index].toPixelIndex, this->_pixelMemoryAddress);
    
    hsv currentPixelColor = this->_tweenHsvColor(startPixel, endPixel, this->_pixelPointers[index].timeout);
    this->_rgbColors[index] = ColorConverter::hsv2rgb(currentPixelColor);
    
    //this->_rgbColors[index].r = rand()%255;
    //this->_rgbColors[index].g = this->_rgbColors[index].b = this->_rgbColors[index].r;
    
    
  }
}


/**
 * display the new color values on the pixel strip
 */

void FireplaceBacklight::displayFrame() {
  unsigned int multiplier = (this->_DIVIDER) - 1;
  unsigned int index;
  cli();
  do {
    index = this->_numPixelColors-1;
    do {
      this->_pixelStrip->sendPixel(this->_rgbColors[multiplier].r, this->_rgbColors[multiplier].g, this->_rgbColors[multiplier].b);
      //this->_pixelStrip->sendPixel(this->_rgbColors[index].r, this->_rgbColors[index].g, this->_rgbColors[index].b);
    } while (index--);
  } while (multiplier--);
  /*
  while (multiplier++ < this->_DIVIDER) {
    for (unsigned int index=0; index<this->_numWorkingPixels; index++) {
      //this->_pixelStrip->sendPixel(this->_rgbColors[0].r, this->_rgbColors[0].g, this->_rgbColors[0].b);
      this->_pixelStrip->sendPixel(this->_rgbColors[index].r, this->_rgbColors[index].g, this->_rgbColors[index].b);
    }
  }
  /* */
  sei();
  
  this->_pixelStrip->show();
  /* */
}

/**
 * find the difference between two pixels based on the current timeout
 *
 * @param startPixel the pixel we started with
 * @param endPixel the pixel we will end with
 * @param timeout the timeout counter that we are going between
 * @return an hsv color struct
 */
/*
hsv FireplaceBacklight::_tweenHsvColor(Pixel startPixel, Pixel endPixel, unsigned int timeout) {
  hsv currentColor;
  
  float percentDone = 1-((float)timeout/(endPixel.timeout));
  
  currentColor.h = this->_tweenValue(startPixel.color.h, endPixel.color.h, percentDone);
  currentColor.s = this->_tweenValue(startPixel.color.s, endPixel.color.s, percentDone);
  currentColor.v = this->_tweenValue(startPixel.color.v, endPixel.color.v, percentDone);
  
  return currentColor;
}
/* */

hsv FireplaceBacklight::_tweenHsvColor(Pixel startPixel, Pixel endPixel, unsigned int timeout) {
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


/**
 * return the percent value between two numbers
 *
 * @param startValue the min value
 * @param endValue the max value
 * @param percent the percent difference between startValue and endValue
 * @return a byte value between 0 and 255
 */
byte FireplaceBacklight::_tweenValue(float startValue, float endValue, float percent) {  
  float floatVal = ((endValue - startValue)*percent + startValue);
  
  byte result = (byte) floatVal;
  return result;
}


