/**
 * LedLifeBar.cpp - Renders a video game-style LED life bar
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */

#include "LedLifeBar.h"
 
/**
 * Create an LED Life Bar
 * 
 * @param ledPin the pin that a Neopixel is connected to
 * @param numLeds the number of Leds in the Life Bar
 */
LedLifeBar::LedLifeBar(const unsigned int ledPin, const unsigned int numLeds) {
  _currentLifePercent = 100.0;
  _lastLifePercent = 100.0;
  _nextLifePercent = 100.0;
  _mode = MODE_NORMAL;
  _ledStrip = new Adafruit_NeoPixel(
    numLeds,
    ledPin,
    NEO_GRB + NEO_KHZ800
  );
  _ledStrip->begin();
  //_ledStrip->show();
}

/**
 * Destroy the LED Life Bar
 */
LedLifeBar::~LedLifeBar() {
  delete _ledStrip;
}

/**
 * Get the current life percent
 * 
 * @return the current life percent value
 */
float LedLifeBar::getLifePercent() {
  return _nextLifePercent;
}

/**
 * Set the life percent
 * 
 * @param percent the life percent value to animate towards
 */
void LedLifeBar::setLifePercent(const float percent) {
  float desiredPercent = percent;
  if (percent < 0) {
    desiredPercent = 0.0;
  }
  if (percent > 100) {
    desiredPercent = 100.0;
  }
  _lastLifePercent = _currentLifePercent;
  _nextLifePercent = desiredPercent;
  //Serial.print("Setting Life percent to: ");
  //Serial.println(_nextLifePercent);
}

/**
 * Get the hue from a life percent.
 * 
 * As the life bar drops, the hue changes
 * 
 * @param percent the percent to calculate the hue from
 */
unsigned int LedLifeBar::_getHueFromPercent(float percent) {
  // return (int) (_currentLifePercent * (_MAX_HUE - _MIN_HUE))/100 + _MIN_HUE; // linear

  // _MIN_HUE at _LOW_LIFE_PERCENT, _MAX_HUE at 100.0
  float totalRange = 100.0 - _LOW_LIFE_PERCENT;
  float currentRange = max(_currentLifePercent - _LOW_LIFE_PERCENT, 0);
  //float workingPercent = 100.0 * currentRange / totalRange;
  return (int) currentRange * (_MAX_HUE - _MIN_HUE) / totalRange + _MIN_HUE;
}

/**
 * Calculate the "tween" animation frame
 */
void LedLifeBar::renderFrame() {
  // calculate the time since the last frame
  if (_currentLifePercent != _nextLifePercent) {
    unsigned long currentTime_ms = millis();
    unsigned long timeSinceLastFrameUpdate_ms = currentTime_ms - _lastFrameUpdateTime_ms;
    Serial.print("time since last frame update: ");
    Serial.println(timeSinceLastFrameUpdate_ms);
    float incrementalChangeInPercent = (100.0 * timeSinceLastFrameUpdate_ms) / (float) _timeForFullLifeDrain_ms;
    Serial.print("Incremental Change in Percent: ");
    Serial.print(timeSinceLastFrameUpdate_ms);
    Serial.print(" / ");
    Serial.print(_timeForFullLifeDrain_ms);
    Serial.print(" = ");
    Serial.println(incrementalChangeInPercent);
    float currentToNextPercentDifference = _nextLifePercent - _currentLifePercent;
    Serial.print("Total Potential Change: ");
    Serial.println(currentToNextPercentDifference);
    if (abs(currentToNextPercentDifference) < abs(incrementalChangeInPercent)) {
      _currentLifePercent = _nextLifePercent;
    } else {
      if (currentToNextPercentDifference < 0) {
        _currentLifePercent -= incrementalChangeInPercent;
      } else {
        _currentLifePercent += incrementalChangeInPercent;
      }
    }
    if (_currentLifePercent < 0) {
      _currentLifePercent = 0.0;
    }
    if (_currentLifePercent > 100.0) {
      _currentLifePercent = 100.0;
    }
    Serial.print("Current Life Percent: ");
    Serial.println(_currentLifePercent);
    
  }
  _lastFrameUpdateTime_ms = millis();
}

/**
 * Draw the current animation fram to the Neopixels
 */
void LedLifeBar::draw() {
  switch (_mode) {
    case MODE_BLINK:
      _drawModeBlink();
    break;
    case MODE_OFF:
      _drawModeOff();
    break;
    case MODE_NORMAL:
    default:
      _drawModeNormal();
    break;
  }
  _ledStrip->show();
}

void LedLifeBar::_drawModeOff() {
  HsvColor hsvBlack = HsvColor();
  hsvBlack.h = _MIN_HUE;
  hsvBlack.s = 255;
  hsvBlack.v = 0;
  
  for (int pixel = 0; pixel < _ledStrip->numPixels(); pixel++) {
    _setPixelHsv(pixel, hsvBlack);
  }
}

void LedLifeBar::_drawModeNormal() {
  unsigned int hue = _getHueFromPercent(_currentLifePercent);
  //Serial.print("Hue: ");
  //Serial.println(hue);
  int aliasedPortion = floor(_ledStrip->numPixels() * (_currentLifePercent / 100.0));
  float aliasedMax = _ledStrip->numPixels() * (_currentLifePercent / 100.0);
  int brightness = MAX_BRIGHTNESS;
  if (_currentLifePercent <= _LOW_LIFE_PERCENT) {
    if (_lowLifeTimeStart < 0) {
      _lowLifeTimeStart = millis();
    }
    unsigned long timeSinceLowLifeStart = millis() - _lowLifeTimeStart;
    brightness = (MAX_BRIGHTNESS / 3.0) * (cos(PI * timeSinceLowLifeStart / 1000)) + (2.0 * MAX_BRIGHTNESS / 3.0);
    //Serial.print("Brightness: ");
    //Serial.println(brightness);
  } else {
    _lowLifeTimeStart = -1;
  }
  int antialiasedValue = ceil(brightness * (aliasedMax - aliasedPortion));

  HsvColor hsvColor = HsvColor();
  hsvColor.h = hue;
  hsvColor.s = 255;
  hsvColor.v = brightness;
  
  HsvColor hsvBlack = HsvColor();
  hsvBlack.h = hue;
  hsvBlack.s = 255;
  hsvBlack.v = 0;
  
  for (int pixel = 0; pixel < _ledStrip->numPixels(); pixel++) {
    if (pixel < aliasedPortion) {
        _setPixelHsv(pixel, hsvColor);
    } else {
      if (pixel == aliasedPortion) {
        HsvColor hsvAntialias = HsvColor();
        hsvAntialias.h = hue;
        hsvAntialias.s = 255;
        hsvAntialias.v = antialiasedValue;
        _setPixelHsv(pixel, hsvAntialias);
      } else {
        _setPixelHsv(pixel, hsvBlack);
      }
    }
  }  
}

void LedLifeBar::_drawModeBlink() {
  int brightness = MAX_BRIGHTNESS;
  if (_lowLifeTimeStart < 0) {
    _lowLifeTimeStart = millis();
  }
  unsigned long timeSinceLowLifeStart = millis() - _lowLifeTimeStart;
  brightness = (MAX_BRIGHTNESS / 3.0) * (cos(PI * timeSinceLowLifeStart / 1000)) + (2.0 * MAX_BRIGHTNESS / 3.0);
    
  HsvColor hsvColor = HsvColor();
  hsvColor.h = _MIN_HUE;
  hsvColor.s = 255;
  hsvColor.v = brightness;
  
  for (int pixel = 0; pixel < _ledStrip->numPixels(); pixel++) {
    _setPixelHsv(pixel, hsvColor);
  }
}


/**
 * Set the current mode
 * 
 * @param mode the mode to set, e.g. MODE_BLINK
 */
void LedLifeBar::setMode(const unsigned int mode) {
  unsigned int desiredMode = mode;
  if (desiredMode > 1002) {
    desiredMode = 1002;
  }
  if (desiredMode < 1000) {
    desiredMode = 1000;
  }
  Serial.print("Setting mode to ");
  Serial.println(desiredMode);
  _mode = desiredMode;
}

/**
 * Get the current mode
 * 
 * @return the current mode
 */
unsigned int LedLifeBar::getMode() {
  return _mode;
}

/**
 * Set the RGB Color of an individual pixel on the LED Strip
 */
void LedLifeBar::_setPixelRgb(unsigned int pixel, RgbColor rgbColor) {
  _ledStrip->setPixelColor(pixel,
    gammaCorrect(rgbColor.r),
    gammaCorrect(rgbColor.g),
    gammaCorrect(rgbColor.b)
  );
}

/**
 * Set the HSV Color of an individual pixel on the LED Strip
 */
void LedLifeBar::_setPixelHsv(unsigned int pixel, HsvColor hsvColor) {
  RgbColor rgbColor = HsvToRgb(hsvColor);
  _setPixelRgb(pixel, rgbColor);
}


