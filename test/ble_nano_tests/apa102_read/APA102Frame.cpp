/**
 * APA102 Frame Linked List
 * 
 * This class takes in RGB color data and stores it
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-28
 */

#include "APA102Frame.h"


APA102Frame::APA102Frame(uint8_t numPixels): _numPixels(numPixels), _nextFrame(NULL), _cursor(0) {
  this->_pixels = (uint32_t*)malloc(sizeof(uint32_t)*this->_numPixels);
}

APA102Frame::~APA102Frame() {
  free(this->_pixels);
  this->_pixels = NULL;
}

APA102Frame* APA102Frame::getNextFrame() {
  return this->_nextFrame;
}

void APA102Frame::setNextFrame(APA102Frame* nextFrame) {
  this->_nextFrame = nextFrame;
}

uint32_t APA102Frame::getPixel(uint8_t index) {
  if (index >= this->_numPixels) {
    return NULL;
  }
  return this->_pixels[index];
}

uint8_t APA102Frame::getNumPixels() {
  return this->_numPixels;
}

void APA102Frame::addPixel(uint32_t pixel) {
  if (this->_cursor < this->_numPixels) {
    this->_pixels[this->_cursor++] = pixel;
  }
}

