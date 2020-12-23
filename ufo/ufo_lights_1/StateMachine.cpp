/**
 * StateMachine.cpp - A State Machine library
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */
 
#include "StateMachine.h"

/**
 * Create a State Machine
 * 
 * @param numStates the total number of states in the state machine
 */
StateMachine::StateMachine(const unsigned int numStates) : _numStates(numStates) {
}

StateMachine::~StateMachine() {
}

/**
 * Move to the next state
 */
void StateMachine::moveToNextState() {
  if (++_currentState >= _numStates) {
    _currentState = 0;
  }
}

/**
 * Get the current state
 * 
 * @return the current state of the statemachine
 */
unsigned int StateMachine::getCurrentState() {
  return _currentState;
}

