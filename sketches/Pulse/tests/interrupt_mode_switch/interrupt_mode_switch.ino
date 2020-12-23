/**
 * Interrupt-based LED Blink Mode switcher - responds to an interrupt handler by 
 * changing the blink mechanism for an LED
 * 
 * @author Tony Gaitatzis 2015-08-01
 * 
 * Things to note: human flicker threshold is around 75Hz. 
 * Therefore we may be able to put the processor to sleep 
 * each 1/75th of a second that is unused by calculations,
 * So long as it doesn't interfere with the quality of the LED brightness
 */
 
#include "LEDBlinker.h"
#include "StateMachine.h"


////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE CONSTANTS
// Change these to change how the program records and processes data
////////////////////////////////////////////////////////////////////////////////


/**
 * System settings
 */
const unsigned int indicatorPin = 13; // LED pin
const unsigned int interruptID = 0;   // pin 3 on Arduino Micro
// sleep settings would go here

/**
 * Animation settings
 */
// animation frame time

/**
 * Human Interaction settings
 */
// debounce timeout

////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

volatile bool stateChanged = false;

LEDBlinker ledBlinker = LEDBlinker(indicatorPin);
StateMachine stateMachine = StateMachine(LEDBlinker::numStates, StateMachine::STATE_OFF);


/**
 * Set up the state machine and initialize the default state
 */
void setup() {
  Serial.begin(9600);
  Serial.println(F("Interrupt Mode Switcher Demo"));
  stateMachine.setSwitchInterrupt(interruptID, handleInterrupt, FALLING);
  ledBlinker.setState(LEDBlinker::STATE_OFF);

}

/**
 * Each loop, we should handle any pending state changes and update the animation frame
 */
void loop() {
  // FIXME: handle debouncing
  // https://www.arduino.cc/en/Tutorial/Debounce
  
  if (stateChanged) {
    stateChanged = false;
    stateMachine.nextState();
    ledBlinker.setState(stateMachine.getState());
  }
  ledBlinker.updateAnimationFrame();

}

/**
 * Handle the interrupt that will trigger the state change
 */
void handleInterrupt() {
  stateMachine.pauseSwitchInterrupt();
  // state change occurred - prepare for new state
  stateChanged = true;
  stateMachine.resumeSwitchInterrupt();
}
