/**
 * InnerLedStrip.cpp - Animates the UFO Inner LED Strip  
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */
 
#include "InnerLedStrip.h"

InnerLedStrip::InnerLedStrip(const unsigned int ledPin, const unsigned int numLeds) {
  _ledStrip = new Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);
  _ledStrip->begin();
  /*
  _ledStrip = new Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);
  _ledStrip->begin();
  _ledStrip->show();
  /* */
}

InnerLedStrip::~InnerLedStrip() {
  delete _ledStrip;
}

void InnerLedStrip::setChaseSpeed(float chaseSpeed) {
  _chaseSpeed = chaseSpeed;
}

void InnerLedStrip::drawNextFrame() {
  Serial.print("Animating inner led ");
  Serial.println(millis());
  switch (_animationMode) {
    case InnerLedStrip::ANIMATION_MODE_FLASHING:
      _drawNextFrameModeFlashing();
    break;
    case InnerLedStrip::ANIMATION_MODE_COLOR_CHASE:
      _drawNextFrameModeColorChase();
    break;
    default:
      _drawNextFrameModeBwChase();
  }
  _ledStrip->show();

  if (++_currentAnimationFrame >= _ledStrip->numPixels() - 1) {
    _currentAnimationFrame = 0;
  }
}

void InnerLedStrip::setAnimationMode(unsigned int animationMode) {
  _animationMode = animationMode;
}

/**
 * Update any internal stuff between frames.
 */
void InnerLedStrip::betweenFrameUpdate() {
  Serial.print("Animating inner led tween ");
  Serial.println(millis());
  _contiguousAnimationFrameCounter++; 
  switch (_animationMode) {
    case InnerLedStrip::ANIMATION_MODE_FLASHING:
      _tweenFrameModeFlashing();
    break;
    default:    
    break;
  }
}

void InnerLedStrip::_drawNextFrameModeBwChase() {
  for (int pixel = 0; pixel < _ledStrip->numPixels(); pixel++) {
    unsigned int animationOffset = (unsigned int) 2 * PI * _currentAnimationFrame / _ledStrip->numPixels();
    float brightness = 255/2.0 + ( 255 * ( cos( ( _numBwHotspots * PI * pixel / _ledStrip->numPixels() ) - animationOffset ) ) / 2.0 );
    _setPixelBrightness(pixel, brightness);
  }
}

/**
 * Smooth the flashing animation by calculating the max brightness between animation frames
 */
void InnerLedStrip::_tweenFrameModeFlashing() {
  _contiguousAnimationFrameCounter++;
  float brightness = 255/2.0 - (255 * cos( (0.001 * PI * pow(_contiguousAnimationFrameCounter, 2) * _flashAnimationSpeed)) / 2.0 );
  _maxFlashBrightness = (int) max(_maxFlashBrightness, brightness);
}

void InnerLedStrip::_drawNextFrameModeFlashing() {
  for (int pixel = 0; pixel < _ledStrip->numPixels(); pixel++) {
    _setPixelBrightness(pixel, _maxFlashBrightness);
  }
  _maxFlashBrightness = 0;
  
}

void InnerLedStrip::_drawNextFrameModeColorChase() {
  // expotent = - sq( pixel - offset ) / (2 * sq(currentAnimationFrame) )
  // 1 * pow( E, exponent );

  unsigned int sectionLength = _ledStrip->numPixels() / _numColoredHotspots;

  static const unsigned int numColors = 3;
  unsigned int hues[numColors];
  hues[0] = 10; // red
  hues[1] = 43; // yellow
  hues[2] = 135; // turquoise
  
  float offsets[numColors];
  offsets[0] = 0;
  offsets[1] = sectionLength / numColors;
  offsets[2] = 2 * sectionLength / numColors;

  unsigned int pixelHues[_ledStrip->numPixels()][numColors];
  
  for (int pixel = 0; pixel < sectionLength; pixel++) {
    for (int color = 0; color < numColors; color++) {
      float offset = offsets[color] + _currentAnimationFrame;
      if (offset < 0) {
        offset = (sectionLength) - offset;
      }
      if (offset >= sectionLength) {
        offset = offset - sectionLength;
      }
      float exponent = - sq( pixel - offset ) / (2 * sq( 0.1 ) );
      pixelHues[pixel][color] = 1 * pow( EULER, exponent );
    }
  }

  for (int section = 0; section < _numColoredHotspots; section++) {
    for (int pixel = 0; pixel < sectionLength; pixel++) {
      HsvColor colors[numColors];
      for (unsigned int color = 0; color < numColors; color++) {
        colors[color] = HsvColor();
        colors[color].h = hues[color];
        colors[color].s = 255;
        colors[color].v = pixelHues[pixel][color];
      }
      HsvColor hsvColor = averageHues(colors, numColors);
      _setPixelHsv(pixel * section, hsvColor);
    }
  }
}

void InnerLedStrip::_setPixelBrightness(unsigned int pixel, unsigned int brightness) {
  RgbColor rgbColor = RgbColor();
  rgbColor.r = brightness;
  rgbColor.g = brightness;
  rgbColor.b = brightness;
  _setPixelRgb(pixel, rgbColor);
}
void InnerLedStrip::_setPixelRgb(unsigned int pixel, RgbColor rgbColor) {
  _ledStrip->setPixelColor(pixel,
    gammaCorrect(rgbColor.r),
    gammaCorrect(rgbColor.g),
    gammaCorrect(rgbColor.b)
  );
}

void InnerLedStrip::_setPixelHsv(unsigned int pixel, HsvColor hsvColor) {
  RgbColor rgbColor = HsvToRgb(hsvColor);
  _setPixelRgb(pixel, rgbColor);
}



