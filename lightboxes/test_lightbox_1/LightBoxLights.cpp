/**
 * LightBoxLights.cpp - controls the light configuration of various internal lights for a light box
 * Created by Tony Gaitatzis, 2018
 * Copyright
 */

#include "LightBoxLights.h"

LightBoxLights::LightBoxLights(const unsigned int pin, const unsigned int numLeds) : _pin(pin), _numLeds(numLeds) {
  _neoPixels = new Adafruit_NeoPixel(_numLeds, _pin, NEO_GRB + NEO_KHZ800);
  _twinkleLights = new TwinkleLights(_numLeds);
  _pulseLights = new PulseLights();
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
  Serial.println(desiredMode);

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
  _neoPixels->setBrightness(brightness);
  _brightness = brightness;
}

void LightBoxLights::setColor(RgbColor color) {
  _color = color;  
}

void LightBoxLights::_initializeModeOff(){
}
void LightBoxLights::_destroyModeOff() {
}
void LightBoxLights::_renderModeOff() {
  analogWrite(_pin, 0);
  /*
  for (int pixel = 0; pixel < _neoPixels->numPixels(); pixel++) {
    _neoPixels->setPixelColor(pixel, 0, 0, 0);
  }
  */
}

void LightBoxLights::_initializeModeSolid() {
}
void LightBoxLights::_destroyModeSolid() {
}
void LightBoxLights::_renderModeSolid() {
  analogWrite(_pin, 255 * _brightness / 255.0);
  /*
  for (int pixel = 0; pixel < _neoPixels->numPixels(); pixel++) {
    _neoPixels->setPixelColor(
      pixel,
      gammaCorrect(_color.r),
      gammaCorrect(_color.g),
      gammaCorrect(_color.b)
    );
  }
  /* */
}

void LightBoxLights::_initializeModeTwinkle() {
}
void LightBoxLights::_destroyModeTwinkle() {
}
void LightBoxLights::_renderModeTwinkle() {
  unsigned int *brightnesses = _twinkleLights->render();
  
  analogWrite(_pin, gammaCorrect(brightnesses[0] * _brightness / 255.0 ));
  /*
  RgbColor color = RgbColor();
  for (int pixel = 0; pixel < _neoPixels->numPixels(); pixel++) {
    color.r = brightnesses[pixel];
    color.g = brightnesses[pixel];
    color.b = brightnesses[pixel];
    _neoPixels->setPixelColor(
      pixel,
      gammaCorrect(color.r),
      gammaCorrect(color.g),
      gammaCorrect(color.b)
    );
  }
  */
}

void LightBoxLights::_initializeModePulse() {
}
void LightBoxLights::_destroyModePulse() {
}
void LightBoxLights::_renderModePulse() {
  int brightness = _pulseLights->render();
  
  analogWrite(_pin, gammaCorrect(brightness * _brightness / 255.0 ));

  /*
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
  /* */
}

