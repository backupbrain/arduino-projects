/*
  FireplaceBacklight.h - Library for powering neopixel fireplace backlight
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.  
*/

#ifndef FireplaceBacklight_h
#define FireplaceBacklight_h

#include "Arduino.h"
#include "ColorConverter.h"
#include "SimpleNeoPixel.h"

struct Pixel {
  hsv color;
  unsigned int timeout; // ms
};


struct PixelPointer {
  byte fromPixelIndex;
  byte toPixelIndex;
  unsigned int timeout;
};


class FireplaceBacklight
{
  public:
    FireplaceBacklight(unsigned char neopixelPin, unsigned int numNeopixels, const unsigned long* pixelMemoryAddress, unsigned int numPixelColors);
    ~FireplaceBacklight();
    void updateFrame();
    void displayFrame();
    
    SimpleNeopixel *_pixelStrip;
    
  private:
    unsigned int _DIVIDER; // divide number of pixel by this number
    unsigned char _neopixelPin; 
    unsigned int _numPixelColors;
    const unsigned long* _pixelMemoryAddress;
    PixelPointer* _pixelPointers;  
    rgb *_rgbColors;
  
    unsigned int _numWorkingPixels;  
    unsigned long lastTime;
    unsigned long currentTime;
	
    void _initialize();
    Pixel _readPixelFromEEPROM(unsigned int pixelNumber, const long unsigned int* memoryAddress);
    Pixel _unpackPixel(unsigned long package);
    hsv _tweenHsvColor(Pixel startPixel, Pixel endPixel, unsigned int timeout);
    byte _tweenValue(float startValue, float endValue, float percent);
    
    
    unsigned long _lastFrameUpdate_ms;
    unsigned long _currentFrameUpdate_ms;
	
};


#endif
