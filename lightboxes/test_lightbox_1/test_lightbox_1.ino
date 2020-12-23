/**
   heartbeat.ino
*/

#include "InternalClock.h"
#include "Heartbeat.h"
#include "MomentaryButton.h"
#include "TurnKnob.h"
#include "PulseLights.h"
#include "StateMachine.h"
#include "LightBoxLights.h"
#include "ColorUtils.h"

// Heartbeat Status
static const unsigned int heartbeatLedPin = 13;
static const unsigned int heartbeatClockTimeout_ms = 500;

// Button configuration
static const unsigned int stateSwitchButtonPin = 2;
static const unsigned int brightnessKnobPin = A0;

// Lights
static const unsigned int lightBoxLedsPin = 6;
static const unsigned int numLeds = 1;
static const unsigned int animationClockTimeout_ms = 33;

// State machine
static const unsigned int numStates = 4;


/**
 * Internal State
 * Don't modify unless you know what you are doing
 */
 

// Set up onboard heartbeat
InternalClock heartbeatTimer = InternalClock(heartbeatClockTimeout_ms);
Heartbeat heartbeat = Heartbeat(heartbeatLedPin);

// Button
MomentaryButton stateSwitchButton = MomentaryButton(stateSwitchButtonPin);

// Brightness knob
TurnKnob brightnessKnob = TurnKnob(brightnessKnobPin);
unsigned int  brightness;

// Set up LightBoxLights and animations
InternalClock animationTimer = InternalClock(animationClockTimeout_ms);

// State machine
StateMachine stateMachine = StateMachine(numStates);


LightBoxLights lights = LightBoxLights(lightBoxLedsPin, numLeds);


void setup() {
  Serial.begin(9600);
  RgbColor color = RgbColor();
  color.r = 255;
  color.g = 0;
  color.b = 255;
  lights.setColor(color);
  lights.setMode(LightBoxLights::MODE_SOLID);
  lights.setBrightness(255);
}

void loop() {
  unsigned int newBrightness = (int) (255 * brightnessKnob.getValue());
  if (newBrightness != brightness) {
    brightness = newBrightness;
    lights.setBrightness(brightness);
  }
  stateSwitchButton.checkForButtonStateChange(onButtonDownEvent, onButtonUpEvent);
  // Update the animation frame when the timer expires
  animationTimer.checkForTimeout(onAnimationFrameReadyEvent);
  // blink the heartbeat when the timer expires
  heartbeatTimer.checkForTimeout(onHeartBeatReadyEvent);
}

/**
 * Heartbeat event. Blink the heartbeat.
 */
void onHeartBeatReadyEvent() {
  heartbeat.beat();
}

/**
 * User clicked the state switch button. Deplete the Life
 */
void onButtonDownEvent() {
}

void onButtonUpEvent() {
  stateMachine.moveToNextState();
  
  switch(stateMachine.getCurrentState()) {
    case 1:
      lights.setMode(LightBoxLights::MODE_TWINKLE);
    break;
    case 2:
      lights.setMode(LightBoxLights::MODE_PULSE);
    break;
    case 3:
      lights.setMode(LightBoxLights::MODE_OFF);
    break;
    default:
      lights.setMode(LightBoxLights::MODE_SOLID);
  }
}


void onAnimationFrameReadyEvent() {
  lights.render();
  
}

