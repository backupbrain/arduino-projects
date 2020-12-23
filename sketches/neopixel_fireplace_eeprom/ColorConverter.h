/*
  ColorConverter.cpp - Library for converting between color space
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/

#ifndef ColorConverter_h
#define ColorConverter_h

#include "Arduino.h"


struct hsv {
    byte h;       // angle in degrees
    byte s;       // percent
    byte v;       // percent
};

struct rgb {
    byte r;
    byte g;
    byte b;
};

class ColorConverter
{
  public:
    
    static hsv rgb2hsv(rgb in);
    static rgb hsv2rgb(hsv in);
};


#endif


