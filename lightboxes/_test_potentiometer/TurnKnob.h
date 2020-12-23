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
