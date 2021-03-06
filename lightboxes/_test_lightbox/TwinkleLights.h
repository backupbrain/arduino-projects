#include <Adafruit_NeoPixel.h>

#ifndef TwinkleLights_h
#define TwinkleLights_h

#include <Adafruit_NeoPixel.h>
#include "ColorUtils.h"

struct TwinklePixel {
  unsigned int brightness;
  long timeout;
};

struct BrightnessRange {
  unsigned int min;
  unsigned int  max;
};

class TwinkleLights
{
  public:
    TwinkleLights(unsigned char neopixelPin, unsigned int numNeopixels);
    ~TwinkleLights();
    void render();
    void setBrightness(const unsigned int brightness);
    void setColor(const RgbColor color);
    
  private:  
    static const unsigned int _TIME_INTERVAL_MIN_MS = 500;  // minimum time between new pixels
    static const unsigned int _TIME_INTERVAL_MAX_MS = 1750; // maximum time between new pixels
    
    BrightnessRange _COLOR_RANGE;
    unsigned char _neopixelPin; 
    TwinklePixel* _pixels;     
    TwinklePixel* _oldPixels;
    RgbColor _color;
  
    void _initialize();
    unsigned int _makeRandomBrightness();
    float _makeRandomValue(float min, float max);
    unsigned int _tweenBrightness(unsigned int previousBrightness, unsigned int nextBrightness, float percent);
    float _tweenValue(float previousValue, float nextValue, float percent);
    int _makeRandomTimeout(int min, int max);
    int _randomizePixel(TwinklePixel &pixel);
    void _copyPixel(TwinklePixel &fromPixel, TwinklePixel &toPixel);
    Adafruit_NeoPixel *_pixelStrip;
  
};

#endif
