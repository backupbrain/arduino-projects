/*
  RgbLED.h - Library for outputting color to a 3 color LED
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.
  
  Usage:
  RgbLED RgbLED = RgbLED(byte rPin, byte gPin, byte bPin);
  RgbLED.setColor(RgbColor rgbColor);
  
*/

#ifndef Color_h
#define Color_h


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

#endif
