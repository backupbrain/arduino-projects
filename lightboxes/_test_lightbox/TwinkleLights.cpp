#include "TwinkleLights.h"

TwinkleLights::TwinkleLights(unsigned char neopixelPin, unsigned int numNeopixels) {
  this->_neopixelPin = neopixelPin;
  
  // initialize _pixels and _oldPixels  
  this->_pixels = new TwinklePixel[numNeopixels];
  this->_oldPixels = new TwinklePixel[numNeopixels];
  
  // make room for the neopixels
  this->_pixelStrip = new Adafruit_NeoPixel((uint16_t) numNeopixels, (uint8_t) this->_neopixelPin, NEO_GRB + NEO_KHZ800);
  this->_pixelStrip->begin();
  
  this->_initialize();
}

TwinkleLights::~TwinkleLights() {
  delete this->_pixelStrip;
  delete this->_oldPixels;
}

void TwinkleLights::_initialize() {
  this->_COLOR_RANGE.max = 255;
  this->_COLOR_RANGE.min = 0;
  
  for (unsigned int index = 0; index < this->_pixelStrip->numPixels(); index++) {
    this->_pixels[index].timeout = 0;
    this->_oldPixels[index].timeout = 0;
  } 
}

unsigned int TwinkleLights::_makeRandomBrightness() {
  unsigned int brightness = round(255 * this->_makeRandomValue(this->_COLOR_RANGE.min, this->_COLOR_RANGE.max));
  return brightness;
}

float TwinkleLights::_makeRandomValue(float min, float max) {
  float multiplier = 1000;
  float retval = 0;
  retval = max-min;
  retval = floor(retval * multiplier);
  retval = float(rand() % (int)retval);
  retval = retval + (multiplier*min);
  retval = retval/multiplier;
  
  return retval;
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

void TwinkleLights::render() {
  long currentTime = millis();
  
  long timeOut;
  long timeScale;
  float percentTween;
  for (unsigned int index = 0; index < this->_pixelStrip->numPixels(); index++) {    
    long timeout = this->_pixels[index].timeout - currentTime;
    
    
    if (timeout > 0) { // tween this pixel if the timeout has not expired
      timeScale = this->_pixels[index].timeout - this->_oldPixels[index].timeout;
      
      if (timeScale <= 0) {
        percentTween = 1.0;
      } else {
        percentTween =  ((float)currentTime - (float)this->_oldPixels[index].timeout) / timeScale;
      }
      int brightness = this->_tweenBrightness(this->_oldPixels[index].brightness, this->_pixels[index].brightness, percentTween);
      
      this->_pixelStrip->setPixelColor(
        index,
        gammaCorrect(_color.r),
        gammaCorrect(_color.g),
        gammaCorrect(_color.b)
      );
      
    } else { // time to make a new pattern for this pixel
      // copy current pixel over to old pixel
      this->_copyPixel(this->_pixels[index], this->_oldPixels[index]);
      
      this->_randomizePixel(this->_pixels[index]);
    }
  }
  this->_pixelStrip->show();
}

void TwinkleLights::setBrightness(const unsigned int brightness) {
  _pixelStrip->setBrightness(brightness);
}

void TwinkleLights::setColor(const RgbColor color) {
  _color = color;
}

