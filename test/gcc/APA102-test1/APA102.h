/**
 * APA102 Parsing/Displaying class
 * 
 * This class takes in RGB color data and converts it to APA102 binary data,
 * and retrieves APA102 binary data and builds an RGB color table
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-26
 */

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#include <stdlib.h>     /* malloc, free, rand */
#include <stdio.h>    /* fprintf */
#endif

#ifndef length
#define length(x) (sizeof(x)/sizeof(*(x)))
#endif

struct RGBColor {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

class APA102Frame {
  public:
    APA102Frame(uint8_t numPixels);
    ~APA102Frame();
  private:
    uint8_t _numPixels;
    RGBColor* pixels;
    APA102 _nextFrame;
};

class APA102 {

  public:
    APA102(uint8_t numPixels);
    //~APA102();
    
    void resetCarat();
    
    void putAPA102(uint32_t packet);
    uint32_t getAPA102(uint8_t index);
    

    void putRGBColor(RGBColor color);
    RGBColor getRGBColor(uint8_t index);
    
    uint32_t RGB2APA102(RGBColor color);
    RGBColor APA1022RGB(uint32_t apa102);

    int parseOctoBytes(uint32_t apa102);

    void printRGB();
    void printAPA102();


  private:
    static uint8_t 
      STATE_HEADER = 0,
      STATE_COLOR = 1;

    int8_t
      frameReadCursor,
      colorReadCursor;

    uint8_t 
      _numPixels,
      _readCarat,
      _writeCarat;

    RGBColor* _colors;

  
};

 
