/**
 * APA102 Parsing/Displaying class
 * 
 * This class takes in RGB color data and converts it to APA102 binary data,
 * and retrieves APA102 binary data and builds an RGB color table
 *
 * This is essentiall a linkedlist holder for an APA102Frame
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-26
 */

#ifndef APA102FrameManager_h
#define APA102FrameManager_h

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#include <stdlib.h>     /* malloc, free, rand */
#include <stdio.h>    /* fprintf */
#endif


#include "RGBColor.h"
#include "APA102Frame.h"



class APA102FrameManager {

  public:
    APA102FrameManager(uint8_t numPixels);
    ~APA102FrameManager();
    
    void resetCarat();
    
    bool hasNext();
    uint32_t readNext();
    

    int parseOctoBytes(uint32_t apa102);


    void printRGB();
    void printAPA102();


    void addFrame();
    APA102Frame* getFrame(uint8_t index);


  private:
    static const uint8_t 
      STATE_HEADER = 0,
      STATE_COLOR = 1;

    int8_t
      _frameReadCursor,
      _colorReadCursor;

    uint8_t 
      _numPixels,
      _readCarat,
      _writeCarat,
      _numFrames;

    APA102Frame* _frameList;

  
};

#endif

 
