/*
  FHTHistory.cpp - Library for storing x,y,z data
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/
#include "FHTHistory.h"


FHTHistory::FHTHistory(unsigned int width, unsigned int height, unsigned int fhtResolution) {
  this->_width = width;
  this->_height = height;
  this->_fhtResolution = fhtResolution;
  
  this->_data = (byte*) malloc (width*height*sizeof(byte));
}


FHTHistory::~FHTHistory() {
    free(this->_data);
    this->_data = NULL;
}


byte* FHTHistory::getData() {
  return this->_data;
}

unsigned int FHTHistory::getWidth() {
  return this->_width;
}
unsigned int FHTHistory::getHeight() {
  return this->_height;
}

void FHTHistory::loadFromFHT(byte* frequencies) {
  this->_push(frequencies);
}


void FHTHistory::_push(byte* values) {
  // push everything in the data array over by one  
  for (int x=this->_width-1; x>0; x--) {
    for (int y=0; y<this->_height; y++) {
      this->setValue(x, y, this->getValue(x-1, y)); 
    }
  } 
  
  // add new data
  int x = 0;
  for (int y=0; y<this->_height; y++) {
    this->setValue(x, y, values[y]);
  }
  
}

byte FHTHistory::getValue(unsigned int x, unsigned int y) {
  return this->_data[x + y * this->_width];
}
void FHTHistory::setValue(unsigned int x, unsigned int y, byte value) {
  this->_data[x + y * this->_width] = value;
}


