#include "PulseTimer.h"

const int buttonDebounceTimeout_ms = 20;
PulseTimer debounceTimer(buttonDebounceTimeout_ms);

const int buttonPin = 0;
int lastButtonValue = LOW;


void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  lastButtonValue = digitalRead(buttonPin);
}

void loop() {
  if (!debounceTimer.update()) { return; }

  
  int currentButtonValue = digitalRead(buttonPin);
  if (currentButtonValue != lastButtonValue) {
    lastButtonValue = currentButtonValue;
    Serial.println("Button Clicked");
  }
}
