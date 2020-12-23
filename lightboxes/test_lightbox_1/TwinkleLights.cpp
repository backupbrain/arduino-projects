#include "TwinkleLights.h"

TwinkleLights::TwinkleLights(unsigned int numPixels) : _numPixels(numPixels) {  
  // initialize _pixels and _oldPixels  
  _pixels = new TwinklePixel[numPixels];
  _oldPixels = new TwinklePixel[numPixels];
  _brightnesses = new unsigned int[numPixels];
  _initialize();
}

TwinkleLights::~TwinkleLights() {
  delete _pixels;
  delete _oldPixels;
  delete _brightnesses;
}

void TwinkleLights::_initialize() {
  this->_COLOR_RANGE.max = 255;
  this->_COLOR_RANGE.min = 0;
  
  for (unsigned int index = 0; index < _numPixels; index++) {
    this->_pixels[index].timeout = 0;
    this->_oldPixels[index].timeout = 0;
  } 
}

unsigned int TwinkleLights::_makeRandomBrightness() {
  unsigned int brightness = random(this->_COLOR_RANGE.min, this->_COLOR_RANGE.max);
  return brightness;
}

unsigned int TwinkleLights::_tweenBrightness(unsigned int startBrightness, unsigned int endBrightness, float percent) {
  return this->_tweenValue(startBrightness, endBrightness, percent);
}

float TwinkleLights::_tweenValue(float previousValue, float nextValue, float percent) {
  float tween = (nextValue-previousValue)*percent + previousValue; 
  return tween;
}

int TwinkleLights::_makeRandomTimeout(int min, int max) {
  return (rand() % (max-min)) + min;
}

int TwinkleLights::_randomizePixel(TwinklePixel &pixel) {
  long currentTime = millis();
  unsigned int brightness = this->_makeRandomBrightness();
  pixel.brightness = brightness;
  pixel.timeout = currentTime + this->_makeRandomTimeout(this->_TIME_INTERVAL_MIN_MS, this->_TIME_INTERVAL_MAX_MS);
}

void TwinkleLights::_copyPixel(TwinklePixel &fromPixel, TwinklePixel &toPixel) {
  toPixel.brightness = fromPixel.brightness;
  toPixel.timeout = fromPixel.timeout;
}

unsigned int *TwinkleLights::render() {
  long currentTime = millis();
  
  long timeOut;
  long timeScale;
  float percentTween;

  for (unsigned int index = 0; index < _numPixels; index++) {    
    long timeout = this->_pixels[index].timeout - currentTime;
    
    
    if (timeout > 0) { // tween this pixel if the timeout has not expired
      timeScale = this->_pixels[index].timeout - this->_oldPixels[index].timeout;
      
      if (timeScale <= 0) {
        percentTween = 1.0;
      } else {
        percentTween =  ((float)currentTime - (float)this->_oldPixels[index].timeout) / timeScale;
      }
      int brightness = this->_tweenBrightness(this->_oldPixels[index].brightness, this->_pixels[index].brightness, percentTween);  
      _brightnesses[index] = brightness;
    } else { // time to make a new pattern for this pixel
      // copy current pixel over to old pixel
      this->_copyPixel(this->_pixels[index], this->_oldPixels[index]);
      
      this->_randomizePixel(this->_pixels[index]);
    }
  }
  return _brightnesses;
}

