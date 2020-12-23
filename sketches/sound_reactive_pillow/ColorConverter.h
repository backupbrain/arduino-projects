/*
  ColorConverter.cpp - Library for converting between color space
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/

#ifndef ColorConverter_h
#define ColorConverter_h

#include "Arduino.h"
#include "Color.h"

class ColorConverter
{
  public:
    
    static RgbColor HsvToRgb(HsvColor in);
    static HsvColor RgbToHsv(RgbColor in);
};

#endif


