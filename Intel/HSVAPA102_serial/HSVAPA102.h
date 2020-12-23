/**
 * The APA102 frame represents the pixel data format, described at:
 * https://cpldcpu.wordpress.com/2014/08/27/apa102/
 * 
 * Each frame is broken down into 4 8-bit parts:
 * [ 8bits | 8bits | 8bits | 8bits ]
 * 
 * The frame parts are represented as such:
 * [ 111 (3bits) | Gamma (5bits) | Blue (8bits) | Green (8bits) | Red (8bits) ]
 * 
 * @author Tony Gaitatzis tonyg@tonygaitatzis.com
 * @date 2015-12-08
 */ 


#ifndef HSVAPA102_h
#define HSVAPA102_h

#include <Arduino.h>
#include "Color.h"

class HSVAPA102 {
  public:

    HSVAPA102();

    uint32_t getHSV();
    uint8_t getHue();
    uint8_t getSaturation();
    uint8_t getValue();

    void loadFromFrame(uint32_t frame);
    
  private:
    uint32_t _frame;
    uint8_t _gamma;

    HsvColor _hsv;

};

#endif
