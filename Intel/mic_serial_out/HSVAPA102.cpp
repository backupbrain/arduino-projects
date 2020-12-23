/**
 * The APA102 frame represents the pixel data format, described at:
 * https://cpldcpu.wordpress.com/2014/08/27/apa102/
 * 
 * Each frame is broken down into 4 8-bit parts:
 * [ 8bits | 8bits | 8bits | 8bits ]
 * 
 * The frame parts are represented as such:
 * [ 111 (3bits) | Gamma (5bits) | Hue (8bits) | Saturation (8bits) | value (8bits) ]
 * 
 * @author Tony Gaitatzis tonyg@tonygaitatzis.com
 * @date 2015-12-08
 */
#include "HSVAPA102.h"

/*
HSVAPA102::HSVAPA102(uint32_t frame) {
  this->_frame = frame;
  this->_frameToHSV(this->_frame);
}

void HSVAPA102::_frameToHSV(uint32_t frame) {
  this->_value = frame & 0xFF;
  this->_saturation = (frame >> 8) & 0xFF;
  this->_hue = (frame >> 16) & 0xFF;
  this->_gamma = (frame >> 24) & 0x1F;
}

uint8_t HSVAPA102::getHue() {
  return this->_hue;
}
uint8_t HSVAPA102::getSaturation() {
  return this->_saturation;
}
uint8_t HSVAPA102::getValue() {
  // we have to multiply the value by the gamma to get the resulting hsv
  uint8_t value = this->_value * this->_gamma;
  return value;
}
/* */


Rectangle::Rectangle (int a, int b) {
  width = a;
  height = b;
}

