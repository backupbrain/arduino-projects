/**
 * This project animates a video-game style LED life bar.
 * A momentary button can incrementally deplete the life bar with each click.
 * Changes in the life bar are animated
 */
#include "InternalClock.h"
#include "MomentaryButton.h"
#include "LedLifeBar.h"
#include "Heartbeat.h"


/**
 * System configuration
 */

// LED animation configuration
const unsigned int neoPixelPin = 9;
const unsigned int numNeoPixels = 32;
static const float lifeBarDecreasePercent = 20.0;
static const unsigned long ledLifeDisplayAnimationClockTimeout_ms = 33;

// heartbeat (onboard LED blink) configuration
const unsigned int heartBeatLedPin = 13;
static const unsigned long heartBeatClockTimeout_ms = 500;

// Button configuration
static const unsigned int stateSwitchButtonPin = 2;


/**
 * Internal State
 * Don't modify unless you know what you are doing
 */

// Animation Clocks
InternalClock ledLifeAnimationTimer = InternalClock(ledLifeDisplayAnimationClockTimeout_ms);

// Heartbeat Clock
InternalClock heartBeatTimer = InternalClock(heartBeatClockTimeout_ms);

// Led Life Bar
LedLifeBar ledLifeBar = LedLifeBar(neoPixelPin, numNeoPixels);

// Button
MomentaryButton stateSwitchButton = MomentaryButton(stateSwitchButtonPin);

// Onboard Heartbeat
Heartbeat heartBeat = Heartbeat(heartBeatLedPin);

static const unsigned int numHits = 4;
static const float hitPercentages[numHits] = {20, 20, 20 ,40};
unsigned int currentHit = 0;

void setup() {
  Serial.begin(9600);
  ledLifeBar.setLifePercent(100.0);
}

void loop() {
  stateSwitchButton.checkForButtonStateChange(onButtonDownEvent, onButtonUpEvent);
  ledLifeAnimationTimer.checkForTimeout(onLedLifeAnimationReadyEvent);
  heartBeatTimer.checkForTimeout(onHeartBeatReadyEvent);
}


/**
 * User clicked the state switch button. Deplete the Life
 */
void onButtonDownEvent() {
  //Serial.println("Button Down!");
}
void onButtonUpEvent() {
  Serial.println("Button Up!");
  unsigned int ledLifeBarMode = ledLifeBar.getMode();
  switch (ledLifeBarMode) {
    case LedLifeBar::MODE_BLINK:
      ledLifeBar.setMode(LedLifeBar::MODE_OFF);
      ledLifeBar.setLifePercent(0.0);
    break;
    case LedLifeBar::MODE_OFF:
      currentHit = 0;
      ledLifeBar.setMode(LedLifeBar::MODE_NORMAL);
      ledLifeBar.setLifePercent(100.0);
    break;
    case LedLifeBar::MODE_NORMAL:
    default:
      if (currentHit < numHits) {
        ledLifeBar.setLifePercent(ledLifeBar.getLifePercent() - hitPercentages[currentHit]);
        currentHit++; 
      } else {
        currentHit = 0;
        ledLifeBar.setMode(LedLifeBar::MODE_BLINK);
      }
    break;
  }
}

/**
 * Render an animation frame
 */
void onLedLifeAnimationReadyEvent() {
  ledLifeBar.renderFrame();
  ledLifeBar.draw();
}

/**
 * Heartbeat event. Blink the heartbeat so we know the program is running properly
 */
void onHeartBeatReadyEvent() {
  heartBeat.beat();
}
