/**
 * APA102 Frame Linked List
 * 
 * This class takes in RGB color data and stores it
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-28
 */


#ifndef APA102Frame_h
#define APA102Frame_h


#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#include <stdlib.h>     /* malloc, free, rand */
#include <stdio.h>    /* fprintf */
#endif

#include "RGBColor.h"

class APA102Frame {
  public:
    APA102Frame(uint8_t numPixels);
    ~APA102Frame();
    APA102Frame* getNextFrame();
    void setNextFrame(APA102Frame* nextFrame);
    uint32_t getPixel(uint8_t index);
    uint8_t getNumPixels();
    void addPixel(uint32_t pixel);
  private:
    uint8_t _numPixels;
    uint8_t _cursor;
    uint32_t* _pixels; 
    APA102Frame* _nextFrame;
};


#endif