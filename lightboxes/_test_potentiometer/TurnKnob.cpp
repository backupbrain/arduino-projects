#include "TurnKnob.h"


/**
 * Instantiate the TurnKnob
 *
 * @param potentiometerPin the analog pin to read from the potentiometer
 */
TurnKnob::TurnKnob(const unsigned int potentiometerPin) : _potentiometerPin(potentiometerPin) {
  pinMode(_potentiometerPin, INPUT);
}

/**
 * Destroy the TurnKnob
 */
TurnKnob::~TurnKnob() {
}

/**
 * Get the value of the Potentiometer
 *
 * @return A float value between 0 and 1.
 */
float TurnKnob::getValue() {
  unsigned long maxValue = 1024;
  unsigned int potentiometerValue = analogRead(_potentiometerPin);
  float value = potentiometerValue / (float) maxValue;
  return value;
}

