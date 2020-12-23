/**
 * The APA102 frame represents the pixel data format, described at:
 * https://cpldcpu.wordpress.com/2014/08/27/apa102/
 * 
 * Each frame is broken down into 4 8-bit parts:
 * [ 8bits | 8bits | 8bits | 8bits ]
 * 
 * The start frame is 4 8-bit zeros (0x0000), and the end frame is 4 8-bit (0xFFFF)
 * Start: [ 0x0000 | 0x0000 | 0x0000 | 0x0000 ]
 * 
 * The led data packets  are represented as such:
 * [ 111 (3bits) | Gamma (5bits) | Hue (8bits) | Saturation (8bits) | value (8bits) ]
 * 
 * End: [ 0xFFFF | 0xFFFF | 0xFFFF | 0xFFFF ]
 * 
 * The whole frame therefore looks like this:
 * [ start | led1 | led2 | ... | ledN | end ]
 * 
 * @author Tony Gaitatzis tonyg@tonygaitatzis.com
 * @date 2015-12-08
 */
#include "HSVAPA102.h"


HSVAPA102::HSVAPA102() {
}

void HSVAPA102::loadFromFrame(uint32_t frame) {
  this->_frame = frame;
  this->_hsv.v = frame & 0xFF;
  this->_hsv.s = (frame >> 8) & 0xFF;
  this->_hsv.h = (frame >> 16) & 0xFF;
  this->_gamma = (frame >> 24) & 0x1F;
}

uint8_t HSVAPA102::getHue() {
  return this->_hsv.h;
}
uint8_t HSVAPA102::getSaturation() {
  return this->_hsv.s;
}
uint8_t HSVAPA102::getValue() {
  // we have to multiply the value by the gamma to get the resulting hsv
  uint8_t value = this->_hsv.v * this->_gamma;
  return value;
}
