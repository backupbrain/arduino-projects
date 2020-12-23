/**
 * LightBoxLights.h - controls the light configuration of various internal lights for a light box
 * Created by Tony Gaitatzis, 2018
 * Copyright
 */

#ifndef LightBoxLights_h
#define LightBoxLights_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "TwinkleLights.h"
#include "PulseLights.h"
#include "ColorUtils.h"

class LightBoxLights {
  public:
    static const unsigned int MODE_OFF = 1000;
    static const unsigned int MODE_SOLID = 1001;
    static const unsigned int MODE_TWINKLE = 1002;
    static const unsigned int MODE_PULSE = 1003;
    
    LightBoxLights(const unsigned int pin, const unsigned int numLeds);
    ~LightBoxLights();

    void setColor(RgbColor color);
    void setMode(const unsigned int mode);
    unsigned int getMode();
    void render();
    void setBrightness(const unsigned int brightness);
    
  private:
    unsigned int _brightness;
    RgbColor _color;
    unsigned int _pin;
    unsigned int _numLeds;
    unsigned int _mode;
    TwinkleLights *_twinkleLights;
    PulseLights *_pulseLights;
    Adafruit_NeoPixel *_neoPixels;

    void _initializeModeOff();
    void _destroyModeOff();
    void _renderModeOff();

    void _initializeModeSolid();
    void _destroyModeSolid();
    void _renderModeSolid();

    void _initializeModeTwinkle();
    void _destroyModeTwinkle();
    void _renderModeTwinkle();

    void _initializeModePulse();
    void _destroyModePulse();
    void _renderModePulse();
};

#endif

