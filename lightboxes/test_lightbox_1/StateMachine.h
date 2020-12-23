/**
 * StateMachine.h - A State Machine library
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */
 
#ifndef StateMachine_h
#define StateMachine_h

#include <Arduino.h>

class StateMachine {
  public:
    // constructors
    StateMachine(const unsigned int numStates);
    ~StateMachine();

    // controllers
    void moveToNextState();
    unsigned int getCurrentState();

  private:
    // internal state
    const unsigned int _numStates;
    unsigned int _currentState = 0;    
};

#endif
