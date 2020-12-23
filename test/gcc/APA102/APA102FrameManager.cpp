/**
 * APA102FrameManager Parsing/Displaying class
 * 
 * This class takes in RGB color data and converts it to APA102 binary data,
 * and retrieves APA102 binary data and builds an RGB color table
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-26
 */

#include "APA102FrameManager.h"

APA102FrameManager::APA102FrameManager(uint8_t numPixels): _numPixels(numPixels), _numFrames(0) {
  this->_frameList = new APA102Frame(this->_numPixels);
  this->resetCarat();
}


APA102FrameManager::~APA102FrameManager() {
  free(this->_frameList);
  this->_frameList = NULL;
}

void APA102FrameManager::resetCarat() {
  this->_frameReadCursor = 0;
  this->_colorReadCursor = -1;
}


bool APA102FrameManager::hasNext() {
    return (this->_frameReadCursor < this->_numFrames-1);
}



 

int APA102FrameManager::parseOctoBytes(uint32_t apa102) {
    // if we are in a header, we should expect 0x0 * 4.

    // on led_global state, we should see some value 0x111NNNN

    // all other led colors are read accordingly

    // if we are in an LED state ...
    // whenever we see a 0x11111111 (x4) we are at a footer

    int state = APA102FrameManager::STATE_COLOR;
    // headers start with 0x00
    bool isHeader = ( ((apa102 >> 24) & 0xff) == 0x00 );


    // if the colorCursor is too big, then we have hit a footer
    if (isHeader) {
      APA102Frame* newFrame = new APA102Frame(this->_numPixels);


      if (this->_numFrames > 0) {
        this->getFrame(this->_numFrames-1)->setNextFrame(newFrame);
      } else {
        this->_frameList = newFrame;
      }
      this->_numFrames++;

      state = STATE_HEADER;

    } else {
      APA102Frame* currentFrame = this->getFrame(this->_numFrames-1);
      currentFrame->addPixel(apa102);
    }


    return state;

}


uint32_t APA102FrameManager::readNext() {
  uint32_t buffer;

  // condition 1: we are at a header frame
  if (this->_colorReadCursor < 0) {
    buffer = 0x00;
  } else if (this->_colorReadCursor < this->_numFrames) { // condition 2: pixel frame
    buffer= this->getFrame(this->_frameReadCursor)->getPixel(this->_colorReadCursor);

  } else { // condition 3: footer frame
    buffer = 0xffffffff;
  }

  // increment cursor
  this->_colorReadCursor++;

  // move to next frame if necessary
  if (this->_colorReadCursor > this->_numFrames) {
    this->_frameReadCursor++;
    this->_colorReadCursor = -1;
  }

 return buffer;
}

APA102Frame* APA102FrameManager::getFrame(uint8_t index) {
  APA102Frame* currentFrame = this->_frameList;
  int count = 0;
  while (currentFrame != NULL) {
    if (count == index) {
      return currentFrame;
    }
    count++;
    currentFrame = currentFrame->getNextFrame();
  }
  return NULL;
}

void APA102FrameManager::addFrame() {
  APA102Frame* child = new APA102Frame(this->_numPixels);
  APA102Frame* parent = this->getFrame(this->_numFrames-1);
  parent->setNextFrame(child);
  this->_numFrames++;
}



