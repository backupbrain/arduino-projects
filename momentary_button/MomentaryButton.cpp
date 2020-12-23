/**
 * MomentaryButton.cpp - Triggers a callback function when a momentary switch is clicked
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */

 
#include "MomentaryButton.h"

/**
 * Create a momentary button controller
 * 
 * @param buttonPin the pin that button is connected to
 */
MomentaryButton::MomentaryButton(const unsigned int buttonPin) : _buttonPin(buttonPin) {
  pinMode(_buttonPin, INPUT);
  _currentState = _previousState = _previousReading = digitalRead(_buttonPin);
  _lastDebounceTime = millis();
}

MomentaryButton::~MomentaryButton() {
}

/**
 * Check for a button state change
 * 
 * @param onButtonClickCallback a callback function that triggers when the button is clicked
 */
void MomentaryButton::checkForButtonStateChange(void (*onButtonDownCallback)(void), void (*onButtonUpCallback)(void)) {
  // read the state of the switch into a local variable:
  int buttonStateSnapshot = digitalRead(_buttonPin);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (buttonStateSnapshot != _previousReading) {
    // reset the debouncing timer
    _lastDebounceTime = millis();
  } 
  
  if ((millis() - _lastDebounceTime) > _debounceDelay_ms) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    _currentState = buttonStateSnapshot;
  }
  
  // set the LED using the state of the button:
  if ((_currentState == STATE_UP) && (_previousState == STATE_DOWN)) {
    _previousState = _currentState;
    onButtonUpCallback();
  }
  if ((_currentState == STATE_DOWN) && (_previousState == STATE_UP)) {
    _previousState = _currentState;
    onButtonDownCallback();
  }

  // save the buttonStateSnapshot.  Next time through the loop,
  // it'll be the _previousState:
  _previousReading = buttonStateSnapshot;
}
