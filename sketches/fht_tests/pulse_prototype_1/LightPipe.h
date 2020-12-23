/*
  LightPipe.h - A library for converting beats into lights
  Created by Tony Gaitatzis, May 31, 2015.
  Released into the public domain.
*/

#ifndef LightPipe_h
#define LightPipe_h

#include "Arduino.h"

class LightPipe
{
  public:
    LightPipe(byte outputPin);
    ~LightPipe();  
    
    void react(byte beatProbability);
    void display();
    byte getBrightness();
  private:
    byte _outputPin;
    byte _currentBrightness;
    unsigned long _lastBeatDetectedTime;
    
    void update(boolean updateBeat, unsigned int beatProbability);
    
    unsigned int _brightessTimeout_ms;
    unsigned int _minBeatTimeout_ms;
    byte _minProbabilityToTriggerBeatTimeout;
    
};


#endif
