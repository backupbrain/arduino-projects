/**
 * LedLifeBar.h - Renders a video game-style LED life bar
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */
 
#ifndef LedLifeBar_h
#define LedLifeBar_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "ColorUtils.h"
 
class LedLifeBar {
  public:
    static const unsigned int MODE_NORMAL = 1000;
    static const unsigned int MODE_BLINK = 1001;
    static const unsigned int MODE_OFF = 1002;
    unsigned int MAX_BRIGHTNESS = 255;
    LedLifeBar(const unsigned int ledPin, const unsigned int numLeds);
    ~LedLifeBar();
    
    float getLifePercent();
    void setLifePercent(const float percent);
    void renderFrame();
    void draw();
    void setMode(const unsigned int mode);
    unsigned int getMode();
  
  private:
    constexpr static const float _LOW_LIFE_PERCENT = 41.0;
    static const unsigned int _MAX_HUE = 70; // Green
    static const unsigned int _MIN_HUE = 0; // Red
    float _lastLifePercent;
    float _currentLifePercent;
    float _nextLifePercent;
    long _lowLifeTimeStart;
    unsigned int _mode;
    unsigned int _hue = 0;
    const unsigned long _timeForFullLifeDrain_ms = 10000;
    unsigned long _lastFrameUpdateTime_ms;
    
    unsigned int _getHueFromPercent(float percent);
    void _drawModeBlink();
    void _drawModeNormal();
    void _drawModeOff();
    void _setPixelRgb(const unsigned int pixel, RgbColor rgbColor);
    void _setPixelHsv(const unsigned int pixel, HsvColor hsvColor);
    Adafruit_NeoPixel *_ledStrip;
};

#endif
