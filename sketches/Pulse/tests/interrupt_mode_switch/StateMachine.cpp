#include "StateMachine.h"


StateMachine::StateMachine(unsigned int numStates, unsigned int startState) {
  this->numStates = numStates;
  this->_states = new unsigned int[this->numStates];
  this->setState(startState);
}

StateMachine::StateMachine(unsigned int numStates, unsigned int interruptID, void (*callback)(void), int triggerMode, unsigned int startState) {
  this->numStates = numStates;
  this->_states = new unsigned int[this->numStates];
  this->setState(startState);
  this->setSwitchInterrupt(interruptID, callback, triggerMode);
}

StateMachine::~StateMachine() {
  delete this->_states;
  this->_states = NULL;
}

void StateMachine::setSwitchInterrupt(unsigned int interruptID, void (*callback)(void), int triggerMode) {
  this->_interruptID = interruptID;
  this->_triggerMode = triggerMode;
  this->_callback = callback;
  attachInterrupt(this->_interruptID, this->_callback, this->_triggerMode);
  
}

void StateMachine::pauseSwitchInterrupt() {
  detachInterrupt(this->_interruptID);
}
void StateMachine::resumeSwitchInterrupt() {
  attachInterrupt(this->_interruptID, this->_callback, this->_triggerMode);
}

void StateMachine::nextState() {
  this->setState(this->_state + 1);
}

void StateMachine::setState(unsigned int state) {
  if (state > this->numStates) { 
   this->setState(state - (this->numStates + 1));
  } else {
    this->_state = state;
  }
}

unsigned int StateMachine::getState() {
  return this->_state;
}


