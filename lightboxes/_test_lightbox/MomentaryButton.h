/**
 * MomentaryButton.h - Triggers a callback function when a momentary switch is clicked
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */
 
#ifndef MomentaryButton_h
#define MomentaryButton_h


#include <Arduino.h>

class MomentaryButton {
  public:
    // button modes
    static const unsigned int STATE_DOWN = HIGH;
    static const unsigned int STATE_UP = LOW;
    
    // constructors
    MomentaryButton(const unsigned int buttonPin);
    ~MomentaryButton();

    // controllers
    void checkForButtonStateChange(void (*onButtonDownCallback)(void), void (*onButtonUpCallback)(void));

  private:
    // internal state
    unsigned int _buttonPin;
    bool _previousReading;
    bool _previousState;
    bool _currentState;
    long _debounceDelay_ms = 50;
    long _lastDebounceTime = 0;
};

#endif
