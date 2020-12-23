

#include "InternalClock.h"
#include "StateMachine.h"
#include "MomentaryButton.h"
#include "TurnKnob.h"
#include "LightBoxLights.h"
#include "Heartbeat.h"


/**
 * System configuration
 */

// control
static const unsigned int stateSwitchButtonPin = 2;
static const unsigned int brightnessKnobPin = A0;

// Heartbeat Status
static const unsigned int heartbeatLedPin = 13;
static const unsigned int heartbeatClockTimeout_ms = 500;

// Lights
static const unsigned int lightBoxLedsPin = 8;
static const unsigned int numLeds = 15;
static const unsigned int animationClockTimeout_ms = 33;

// State machine
static const unsigned int numStates = 4;

/**
 * Internal State
 * Don't modify unless you know what you are doing
 */

// State switch button
MomentaryButton stateSwitchButton = MomentaryButton(stateSwitchButtonPin);

// Brightness knob
TurnKnob brightnessKnob = TurnKnob(brightnessKnobPin);
unsigned int  brightness;

// Set up onboard heartbeat
InternalClock heartbeatTimer = InternalClock(animationClockTimeout_ms);
Heartbeat heartbeat = Heartbeat(heartbeatClockTimeout_ms);

// State machine
StateMachine stateMachine = StateMachine(numStates);

// Set up LightBoxLights and animations
LightBoxLights lights = LightBoxLights(lightBoxLedsPin, numLeds);
InternalClock animationTimer = InternalClock(animationClockTimeout_ms);




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  RgbColor color = RgbColor();
  color.r = 255;
  color.g = 0;
  color.b = 255;
  lights.setColor(color);
}

void loop() {
  unsigned int newBrightness = (int) (255 * brightnessKnob.getValue());
  if (newBrightness != brightness) {
    brightness = newBrightness;
    lights.setBrightness(brightness);
    Serial.print("brightness: ");
    Serial.println(brightness);
  }
  stateSwitchButton.checkForButtonStateChange(onButtonDownCallback, onButtonUpCallback);
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


void onButtonDownCallback() {
  
}
void onButtonUpCallback() {
  Serial.println("Button Click");
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

