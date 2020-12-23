/**
 * LightBoxLights.cpp - controls the light configuration of various internal lights for a light box
 * Created by Tony Gaitatzis, 2018
 * Copyright
 */

#include "LightBoxLights.h"

LightBoxLights::LightBoxLights(const unsigned int pin, const unsigned int numLeds) : _pin(pin), _numLeds(numLeds) {
}

LightBoxLights::~LightBoxLights() {
  delete _neoPixels;
  delete _twinkleLights;
  delete _pulseLights;
}

void LightBoxLights::setMode(const unsigned int mode) {
  unsigned int desiredMode = mode;
  if (desiredMode < MODE_OFF) {
    desiredMode = MODE_OFF;
  }
  if (desiredMode > MODE_PULSE) {
    desiredMode = MODE_PULSE;
  }
  _mode = desiredMode;

  _destroyModeOff();
  _destroyModeSolid();
  _destroyModeTwinkle();
  _destroyModePulse();
  switch (_mode) {
    case MODE_SOLID:
      _initializeModeSolid();
    break;
    case MODE_TWINKLE:
      _initializeModeTwinkle();
    break;
    case MODE_PULSE:
      _initializeModePulse();
    break;
    case MODE_OFF:
    default:
      _initializeModeOff();
    break;
  }
}

unsigned int LightBoxLights::getMode() {
  return _mode;
}

void LightBoxLights::render() {
  switch (_mode) {
    case MODE_SOLID:
      _renderModeSolid();
    break;
    case MODE_TWINKLE:
      _renderModeTwinkle();
    break;
    case MODE_PULSE:
      _renderModePulse();
    break;
    case MODE_OFF:
    default:
      _renderModeOff();
    break;
  }
}

void LightBoxLights::setBrightness(const unsigned int brightness) {
  switch (_mode) {
    case MODE_SOLID:
      _neoPixels->setBrightness(brightness);
    break;
    case MODE_TWINKLE:
      _twinkleLights->setBrightness(brightness);
    break;
    case MODE_PULSE:
      _neoPixels->setBrightness(brightness);
    break;
    case MODE_OFF:
    default:
    break;
  }
}

void LightBoxLights::setColor(RgbColor color) {
  _color = color;
  switch (_mode) {
    case MODE_TWINKLE:
      _twinkleLights->setColor(color);
    break;
    default:
    break;
  }
  
}

void LightBoxLights::_initializeModeOff(){
  _neoPixels = new Adafruit_NeoPixel(_numLeds, _pin, NEO_GRB + NEO_KHZ800);
}
void LightBoxLights::_destroyModeOff() {
  delete _neoPixels;  
}
void LightBoxLights::_renderModeOff() {
  for (int pixel = 0; pixel < _neoPixels->numPixels(); pixel++) {
    _neoPixels->setPixelColor(pixel, 0, 0, 0);
  }
}

void LightBoxLights::_initializeModeSolid() {
  _neoPixels = new Adafruit_NeoPixel(_numLeds, _pin, NEO_GRB + NEO_KHZ800);
}
void LightBoxLights::_destroyModeSolid() {
  delete _neoPixels;    
}
void LightBoxLights::_renderModeSolid() {
  for (int pixel = 0; pixel < _neoPixels->numPixels(); pixel++) {
    _neoPixels->setPixelColor(
      pixel,
      gammaCorrect(_color.r),
      gammaCorrect(_color.g),
      gammaCorrect(_color.b)
    );
  }
}

void LightBoxLights::_initializeModeTwinkle() {
  _twinkleLights = new TwinkleLights(_pin, _numLeds);
}
void LightBoxLights::_destroyModeTwinkle() {
  delete _twinkleLights;  
}
void LightBoxLights::_renderModeTwinkle() {
  _twinkleLights->render();  
}

void LightBoxLights::_initializeModePulse() {
  _pulseLights = new PulseLights();
  _neoPixels = new Adafruit_NeoPixel(_numLeds, _pin, NEO_GRB + NEO_KHZ800);
}
void LightBoxLights::_destroyModePulse() {
  delete _pulseLights;
  delete _neoPixels;
}
void LightBoxLights::_renderModePulse() {
  int brightness = _pulseLights->render();
  RgbColor color = RgbColor();
  color.r = brightness;
  color.g = brightness;
  color.b = brightness;
  for (int pixel = 0; pixel < _neoPixels->numPixels(); pixel++) {
    _neoPixels->setPixelColor(
      pixel,
      gammaCorrect(color.r),
      gammaCorrect(color.g),
      gammaCorrect(color.b)
    );
  }
}
