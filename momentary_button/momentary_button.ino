#include "MomentaryButton.h"

static const unsigned int statusLedPin = 13;

// Button configuration
static const unsigned int stateSwitchButtonPin = 2;
MomentaryButton stateSwitchButton = MomentaryButton(stateSwitchButtonPin);

void setup() {
  Serial.begin(9600);
}

void loop() {
  stateSwitchButton.checkForButtonStateChange(onButtonDownEvent, onButtonUpEvent);
}

/**
 * User clicked the state switch button. Deplete the Life
 */
void onButtonDownEvent() {
  Serial.print("Button Down: ");
  Serial.println(millis());
  digitalWrite(statusLedPin, LOW);
}
void onButtonUpEvent() {
  Serial.print("Button Up: ");
  Serial.println(millis());
  digitalWrite(statusLedPin, HIGH);
}
