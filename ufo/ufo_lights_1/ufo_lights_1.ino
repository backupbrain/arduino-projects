/**
 * This project animates a disco ball UFO with a NeoPixel strip.
 * A momentary button can change the current animation of the LEDs
 */
#include "InternalClock.h"
#include "StateMachine.h"
#include "MomentaryButton.h"
#include "InnerLedStrip.h"
#include "OuterLedStrip.h"
#include "Heartbeat.h"

/**
 * System configuration
 */

// input/output pins
static const unsigned int stateSwitchButtonPin = 2;
static const unsigned int heartbeatLedPin = 13;
static const unsigned int innerLedStripPin = 9;
static const unsigned int outerLedStripPin = 10;

// LED animation configuration
static const unsigned int numInnerLeds = 11;
static const unsigned int numOuterLeds = 11;
static const unsigned int animationClockTimeout_ms = 50;
static const unsigned int animationTweenClockTimeout_ms = 20;

// heartbeat (onboard LED blink) configuration
static const unsigned int heartbeatClockTimeout_ms = 500;

// configuration
static const bool isInnerLedsActive = true;
static const bool isOuterLedsActive = false;

/**
 * Internal State
 * Don't modify unless you know what you are doing
 */

// Animation Clocks
InternalClock animationTimer = InternalClock(animationClockTimeout_ms);
InternalClock animationTweenTimer = InternalClock(animationTweenClockTimeout_ms);

// Heartbeat clock
InternalClock heartbeatTimer = InternalClock(heartbeatClockTimeout_ms);

// State switch button
MomentaryButton stateSwitchButton = MomentaryButton(stateSwitchButtonPin);

// Track the current state
static const unsigned int numStates = 4;
StateMachine stateMachine = StateMachine(numStates);

// Set up UFO LED strip
InnerLedStrip innerLedStrip = InnerLedStrip(innerLedStripPin, numInnerLeds);
//OuterLedStrip outerLedStrip = OuterLedStrip(outerLedStripPin, numOuterLeds);

// Set up onboard heartbeat
Heartbeat heartbeat = Heartbeat(heartbeatLedPin);

void setup() {
  Serial.begin(9600);
  if (isInnerLedsActive) {
    innerLedStrip.setAnimationMode(InnerLedStrip::ANIMATION_MODE_BW_CHASE);
  }
  /*
  if (isOuterLedsActive) {
    outerLedStrip.setAnimationMode(InnerLedStrip::ANIMATION_MODE_BW_CHASE);
  }
  /* */
}

void loop() {
  // check for a button click
  stateSwitchButton.checkForButtonStateChange(onButtonClickEvent);
  // Update the animation frame when the timer expires
  animationTimer.checkForTimeout(onAnimationFrameReadyEvent);
  // Smooth the animaton by calculataing the "tween" animation frames
  animationTweenTimer.checkForTimeout(onAnimationTweenFrameReadyEvent);
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
 * Animation Frame is ready. Draw the next frame
 */
void onAnimationFrameReadyEvent() {
  if (isInnerLedsActive) {
    innerLedStrip.drawNextFrame();
  }
  /*
  if (isOuterLedsActive) {
    outerLedStrip.drawNextFrame();
  }
  /* */
}

/**
 * Smooth the animation by calculating "tween" frames
 */
void onAnimationTweenFrameReadyEvent() {
  // update anything that needs updating
  if (isInnerLedsActive) {
    innerLedStrip.betweenFrameUpdate();
  }
  /*
  if (isOuterLedsActive) {
    outerLedStrip.betweenFrameUpdate();
  }
  /* */
}


/**
 * User clicked the state switch button. Change the state.
 */
void onButtonClickEvent() {
  stateMachine.moveToNextState();
  /*
  switch(stateMachine.getCurrentState()) {
    case 1:
      if (isInnerLedsActive) {
        innerLedStrip.setAnimationMode(InnerLedStrip::ANIMATION_MODE_FLASHING);
      }
      if (isOuterLedsActive) {
        outerLedStrip.setAnimationMode(InnerLedStrip::ANIMATION_MODE_FLASHING);
      }
    break;
    case 2:
      if (isInnerLedsActive) {
        innerLedStrip.setAnimationMode(InnerLedStrip::ANIMATION_MODE_BW_CHASE);
      }
      if (isOuterLedsActive) {
        outerLedStrip.setAnimationMode(InnerLedStrip::ANIMATION_MODE_BW_CHASE);
      }
    break;
    case 3:
      if (isInnerLedsActive) {
        innerLedStrip.setAnimationMode(InnerLedStrip::ANIMATION_MODE_COLOR_CHASE);
      }
      if (isOuterLedsActive) {
        outerLedStrip.setAnimationMode(InnerLedStrip::ANIMATION_MODE_COLOR_CHASE);
      }
    break;
    default:
      if (isInnerLedsActive) {
        innerLedStrip.setAnimationMode(InnerLedStrip::ANIMATION_MODE_BW_CHASE);
      }
      if (isOuterLedsActive) {
        outerLedStrip.setAnimationMode(InnerLedStrip::ANIMATION_MODE_BW_CHASE);
      }
  }
  /* */
}

