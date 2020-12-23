/*
  PixelEqualizer.h - Library for displaying equalizer on a NeoPixel screen
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.
*/

#ifndef PixelEqualizer_h
#define PixelEqualizer_h

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

class PixelEqualizer
{
  public:
    PixelEqualizer(unsigned int neoPixelPin, unsigned int xResolution, unsigned int yResolution);
    ~PixelEqualizer();
    void initialize();
    void display(float* spectrum);
  private:
    unsigned int _neoPixelPin = 3;
    unsigned int _xRes = 8;
    unsigned int _yRes = 8;
    Adafruit_NeoPixel* pixelStrip;
    
    
  float SPECTRUM_MIN_DB = 30.0;          // Audio intensity (in decibels) that maps to low LED brightness.
  float SPECTRUM_MAX_DB = 60.0;          // Audio intensity (in decibels) that maps to high LED brightness.
  
  float* hues;
  
  uint32_t _pixelHSVtoRGBColor(float hue, float saturation, float value);
};

#endif

