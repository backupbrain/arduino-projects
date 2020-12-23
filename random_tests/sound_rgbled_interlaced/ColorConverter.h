/*
  ColorConverter.cpp - Library for converting between color space
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/

#ifndef ColorConverter_h
#define ColorConverter_h

#include "Arduino.h"


typedef struct RgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

typedef struct HsvColor
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;

class ColorConverter
{
  public:
    
    static RgbColor HsvToRgb(HsvColor hsv);
    static HsvColor RgbToHsv(RgbColor rgb);
};

#endif


