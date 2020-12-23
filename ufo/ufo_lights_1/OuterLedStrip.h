/**
 * OuterLedStrip.h - Animates the UFO Outer LED Strip  
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */
 
#ifndef OuterLedStrip_h
#define OuterLedStrip_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "ColorUtils.h"

#ifndef EULER
#define EULER 2.718281828459045235360287471352
#endif 

class OuterLedStrip {
  public:
    // Animation modes
    static const unsigned int ANIMATION_MODE_BW_CHASE = 1000;
    static const unsigned int ANIMATION_MODE_FLASHING = 1001;
    static const unsigned int ANIMATION_MODE_COLOR_CHASE = 1002;

    // constructors
    OuterLedStrip(const unsigned int ledPin, const unsigned int numLeds);
    ~OuterLedStrip();

    // Change the animation parameters
    void setAnimationMode(const unsigned int animationMode);
    void setChaseSpeed(float chaseSpeed);
    void betweenFrameUpdate();
    void drawNextFrame();

  private:
    // animation state
    unsigned int _currentAnimationMode;
    unsigned int _animationMode;
    unsigned long _lastFrameTime;
    unsigned long _currentFrameTime;

    // Animations
    void _drawNextFrameModeBwChase();
    void _drawNextFrameModeFlashing();
    void _drawNextFrameModeColorChase();
    void _tweenFrameModeFlashing();

    // Control the pixels
    void _setPixelBrightness(const unsigned int pixel, const unsigned int brightness);
    void _setPixelRgb(const unsigned int pixel, RgbColor rgbColor);
    void _setPixelHsv(const unsigned int pixel, HsvColor hsvColor);

    // LED and animation internal state
    Adafruit_NeoPixel *_ledStrip;
    static const unsigned int _numColoredHotspots = 5;
    static const unsigned int _numBwHotspots = 2;
    static constexpr float _flashAnimationSpeed = 0.1;
    float _chaseSpeed;
    unsigned long _currentAnimationFrame = 0;
    unsigned long _contiguousAnimationFrameCounter = 0;
    unsigned int _maxFlashBrightness = 0;
};

#endif
