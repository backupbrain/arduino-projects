/*
  BeatAnalyzer.h - A library for doing beat detection from FHT
  Created by Tony Gaitatzis, May 31, 2015.
  Released into the public domain.
*/

#ifndef BeatAnalyzer_h
#define BeatAnalyzer_h

#include "Arduino.h"

class BeatAnalyzer
{
  public:
    BeatAnalyzer();
    ~BeatAnalyzer();  
    
    void getAverageVolumeForFrequency(byte* data, unsigned int historyChannelLength);
    byte calculateBeatProbability(byte volume);
  private:
    float _averageVolume;
    float _maxVolume;
};


#endif
