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

class WaterfallPlotter
{
  public:
  
    
    WaterfallPlotter(unsigned int width, unsigned int height, uint8_t* neopixelPins, uint8_t numPins);
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
    int _numPins;
    byte* _data;
    float _maxValue;
    
    uint8_t* _neopixelPins;

    Adafruit_NeoPixel** _pixelStrips;
    
    RgbColor _getPixelColor(unsigned int x, unsigned int y, byte intensity);
    int _getValue(unsigned int x, unsigned int y);
};

#endif

