/**
 * TurnKnob.cpp - Reads a potentiometer value as a float between 0.0 and 1.0
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */

#ifndef TurnKnob_h
#define TurnKnob_h

#include <Arduino.h>

class TurnKnob {
  public:
    TurnKnob(const unsigned int potentiometerPin);
    ~TurnKnob();
    float getValue();
  
  private:
    unsigned int _potentiometerPin;
};

#endif
