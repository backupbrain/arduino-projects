/*
  FHTChannelHistory.cpp - Library for storing a single channel frequency volume changes over time
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/
#include "FHTChannelHistory.h"

FHTChannelHistory::FHTChannelHistory(unsigned int historyBufferLength) {
  this->_historyBufferLength = historyBufferLength;
  
  this->_data = (byte*) malloc (this->_historyBufferLength*sizeof(byte));
  for (unsigned int historyPosition = 0; historyPosition < this->_historyBufferLength; historyPosition++) {
    this->_data[historyPosition] = 0;
  }
}


FHTChannelHistory::~FHTChannelHistory() {
    free(this->_data);
    this->_data = NULL;
}


byte* FHTChannelHistory::getData() {
  return this->_data;
}

unsigned int FHTChannelHistory::getHistoryBufferLength() {
  return this->_historyBufferLength;
}


void FHTChannelHistory::loadFromFHT(byte frequencyVolume) {
  for (unsigned int historyPosition = this->_historyBufferLength-1; historyPosition > 0; historyPosition--) {
    this->_data[historyPosition] = this->_data[historyPosition-1];
  }
    
  // add new data
  unsigned int historyPosition = 0;
  this->setValue(historyPosition, frequencyVolume);
  
}


int FHTChannelHistory::getValue(unsigned int historyPosition) {
  return this->_data[historyPosition];
}
void FHTChannelHistory::setValue(unsigned int historyPosition, byte value) {
  this->_data[historyPosition] = value;
}


