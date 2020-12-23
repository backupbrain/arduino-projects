/**
 * This Arduino state machine changes state whenever an interrupt is triggered
 * It can be programmed to understand an arbitrary number of states
 * and how the callback is triggered and through which interrupt striggers a state change
 * 
 * @author Tony Gaitatzis tonyg@tonygaitatzis.com
 * @version 1.0  2015/08/01
 */
 
#ifndef StateMachine_h
#define StateMachine_h

#include <Arduino.h>


class StateMachine {
  public:
    unsigned int numStates;        /*!< Number of states */
    
    /**
     * Instantiate with a number of states (including off) and the initial state (default off)
     * @param the number of states to support (including off)
     * @param the initial state (default StateMachine::STATE_OFF)
     */
    StateMachine(unsigned int numStates, unsigned int startState = 0);

    /**
     * Instantiate with a number of states (including off) and the initial state (default off)
     * @param the number of states to support (including off)
     * @param the attachInterrupt() function callback handler.  This cannot be a class method
     * @param the attachInterrupt() trigger, eg CHANGE, RISING, FALLING, HIGH, LOW
     * @param the initial state (default StateMachine::STATE_OFF)
     */
    StateMachine(unsigned int numStates, unsigned int interruptID, void (*callback)(void), int triggerMode, unsigned int startState = 0);

    /**
     * Destroy the state machine
     */
    ~StateMachine();

    /**
     * Move to the next state.  The state will roll over to state 0 when the highest state has been reached
     */
    void nextState();

    /**
     * Set a specific state. The state will roll over to state 0 when the highest state has been reached.
     */
    void setState(unsigned int state);

    
    /**
     * Get the current state
     */
    unsigned int getState();  
      
    /**
     * Set the interrupt switch.
     * @param the attachInterrupt() interrupt id.  
     * @param the attachInterrupt() function callback that's run when the interrupt is triggered. 
     * @param the attachInterrupt() trigger mode, eg CHANGE, RISING, FALLING, LOW, and HIGH
     */
    void setSwitchInterrupt(unsigned int interruptID, void (*callback)(void), int triggerMode);
    
    /**
     * Pause the interrupt handler.  No interrupts will be handled
     */
    void pauseSwitchInterrupt();
    
    /**
     * Resume the interrupt handler.  Interrupts will be handled again
     */
    void resumeSwitchInterrupt();
  private:
    unsigned int _state;   /*!< Current state */
    unsigned int* _states;          /*!< Available states */
    unsigned int _triggerMode;      /*!< attachInterrupt() interrupt trigger mode */
    unsigned int _interruptID;      /*!< attachInterrupt() interrupt id */
    void (*_callback)(void);        /*!< attachInterrupt() callback function */
};

#endif

