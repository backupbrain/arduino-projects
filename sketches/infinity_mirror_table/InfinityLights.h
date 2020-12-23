/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef InfinityLights_h
#define InfinityLights_h

#include "Arduino.h"
#include "SimpleNeopixel.h"

class InfinityLights
{
  public:
    InfinityLights(unsigned int numPixels, uint8_t dataPin);
    ~InfinityLights();  
    
    int getNumPixels();
	
    void update(byte nawValue);
    void display();

  private:    
    uint8_t _dataPin;
    unsigned int _numPixels;

    
    SimpleNeopixel* _pixelStrip;
    byte* _lightBrigtnesses;  // half the size of the pixelStrip

};

#endif

