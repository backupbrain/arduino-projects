/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef WaterfallPlotter_h
#define WaterfallPlotter_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "ColorConverter.h"
#include "Image.h"

class WaterfallPlotter
{
  public:
  
    
    WaterfallPlotter(unsigned int width, unsigned int height, uint8_t neopixelPin, byte* imageData);
    ~WaterfallPlotter();  
    
    int getWidth();
    int getHeight();
    void setMaxValue(float maxValue);
    float getMaxValue();
  
    void update(byte* data);
    void display();

  private:
    int _width;
    int _height;
    byte* _data;
    float _maxValue;
    
    uint8_t _neopixelPin;

    Adafruit_NeoPixel* _pixelStrip;
    Image* _image;
    
    RgbColor _getPixelColor(unsigned int x, unsigned int y, byte intensity);
    int _getValue(unsigned int x, unsigned int y);
};

#endif

