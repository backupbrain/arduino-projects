/*
  RgbLED.h - Library for outputting color to a 3 color LED
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.
  
  Usage:
  RgbLED RgbLED = RgbLED(byte rPin, byte gPin, byte bPin);
  RgbLED.setColor(RgbColor rgbColor);
  
*/

#ifndef RgbLED_h
#define RgbLED_h

#include "Arduino.h"
#include "ColorConverter.h"


#define LED_TYPE_COMMON_ANODE false
#define LED_TYPE_COMMON_CATHODE true

class RgbLED
{
  public:
    RgbLED(char port, boolean ledType=LED_TYPE_COMMON_ANODE);
    ~RgbLED();
    void setColor(RgbColor color);
    void _pwm(byte r, byte g, byte b);
    void output();
    RgbColor getColor();
  private:
    boolean _ledType;
    RgbColor _rgbColor;
    
    unsigned long _port;
    unsigned int _ddr;
};

#endif
