/**
 * TurnKnob / Potentiometer Tester:
 * Demo: https://www.tinkercad.com/things/egUB4hdWLod-object-oriented-potentiometer/editel
 * @author "Tony Gaitatzis" <backupbrain@gmail.com>
 * @date 2018-05-05
 */
 
#include "TurnKnob.h"

/**
 * System configuration
 */
static const unsigned int potentiometerPin = A0;
static const unsigned int statusLedPin = 13;

TurnKnob turnKnob = TurnKnob(potentiometerPin);

/**
 * System
 */
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  float potentiometerValue = turnKnob.getValue();
  Serial.println(potentiometerValue);
  delay(500);
}
