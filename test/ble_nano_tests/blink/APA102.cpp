/**
 * APA102 Parsing/Displaying class
 * 
 * This class takes in RGB color data and converts it to APA102 binary data,
 * and retrieves APA102 binary data and builds an RGB color table
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-26
 */

#include "APA102.h"

APA102::APA102(uint8_t numPixels) {
  this->_numPixels = numPixels;
  this->_colors = (RGBColor*)malloc(sizeof(RGBColor)*numPixels);
}

APA102::~APA102() {
  free(this->_colors);
  this->_colors = NULL;
}

void APA102::resetReadCarat() {
  this->_readCarat = 0;
}
void APA102::resetWriteCarat() {
  this->_writeCarat = 0;
}



void APA102::putRGBColor(RGBColor color) {
  this->_colors[_writeCarat++] = color;
}

RGBColor APA102::getRGBColor(uint8_t index) {
  return this->_colors[index];
}


void APA102::putAPA102(uint32_t packet) {
  // decode apa102 packet
  RGBColor color = this->APA1022RGB(packet);
  this->putRGBColor(color);
}

uint32_t APA102::getAPA102(uint8_t index) {
  RGBColor color = this->getRGBColor(index);
  return this->RGB2APA102(color);
}


uint32_t APA102::RGB2APA102(RGBColor color) {
  // begin with zeros
  uint32_t apa102 = 0xff << 24;
  // pack in as RGB
  apa102 |= color.red << 16;
  apa102 |= color.green << 8;
  apa102 |= color.blue;
}
RGBColor APA102::APA1022RGB(uint32_t apa102) {
  // unpack as RGB
  RGBColor color;
  color.red = (apa102 >> 16) && 0xff;
  color.green = (apa102 >> 8) && 0xff;
  color.blue = apa102 && 0xff;
}
 

