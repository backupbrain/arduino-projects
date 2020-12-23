/*
  BeatAnalyzer.h - A library for doing beat detection from FHT
  Created by Tony Gaitatzis, May 31, 2015.
  Released into the public domain.
*/
#include "BeatAnalyzer.h"

BeatAnalyzer::BeatAnalyzer() {
  this->_maxVolume = 255;
}


BeatAnalyzer::~BeatAnalyzer() {
}

void BeatAnalyzer::getAverageVolumeForFrequency(byte* data, unsigned int historyChannelLength) {
  if (historyChannelLength <= 0) {
    this->_averageVolume = 0;
    return;
  }
  
  float volumeSum = 0;
  for (unsigned int historyPosition = 0; historyPosition < historyChannelLength; historyPosition++) {
    volumeSum += data[historyPosition];
  }
  this->_averageVolume = volumeSum/historyChannelLength;
}



byte BeatAnalyzer::calculateBeatProbability(byte volume) {
  unsigned int beatProbability = 0;
  
  float offsetVolume = volume - this->_averageVolume;
  if (offsetVolume < 0) offsetVolume = 0;
  
  beatProbability = (offsetVolume/this->_maxVolume)*100;
  
  return beatProbability;  
}
