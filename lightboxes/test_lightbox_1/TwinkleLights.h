
#ifndef TwinkleLights_h
#define TwinkleLights_h

#include <Arduino.h>

struct TwinklePixel {
  int brightness;
  long timeout;
};

struct BrightnessRange {
  unsigned int min;
  unsigned int max;
};

class TwinkleLights
{
  public:
    TwinkleLights(unsigned int numPixels);
    ~TwinkleLights();
    unsigned int *render();
    
  private:  
    static const unsigned int _TIME_INTERVAL_MIN_MS = 500;  // minimum time between new pixels
    static const unsigned int _TIME_INTERVAL_MAX_MS = 1750; // maximum time between new pixels
    
    BrightnessRange _COLOR_RANGE;
    unsigned int _numPixels;
    TwinklePixel* _pixels;     
    TwinklePixel* _oldPixels;
    unsigned int *_brightnesses;
  
    void _initialize();
    unsigned int _makeRandomBrightness();
    unsigned int _tweenBrightness(unsigned int previousBrightness, unsigned int nextBrightness, float percent);
    float _tweenValue(float previousValue, float nextValue, float percent);
    int _makeRandomTimeout(int min, int max);
    int _randomizePixel(TwinklePixel &pixel);
    void _copyPixel(TwinklePixel &fromPixel, TwinklePixel &toPixel);
  
};

#endif
