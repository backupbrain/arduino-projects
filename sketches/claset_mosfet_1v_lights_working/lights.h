#include "gamma.h"

static uint8_t ledPin = 0;


static uint8_t LIGHT_STATE_ON = 0x100;
static uint8_t LIGHT_STATE_OFF = 0x101;
uint8_t lightState;

static uint8_t LIGHT_BRIGHTNESS_MAX = 255;
static uint8_t LIGHT_BRIGHTNESS_MIN = 0;
static uint8_t LIGHT_ON_TRANSITION_STEP = 8;
static uint8_t LIGHT_OFF_TRANSITION_STEP = 2;

static unsigned long LIGHTS_ANIMATION_TIMEOUT_MS = 1;

int lightBrightnessMax = LIGHT_BRIGHTNESS_MAX;

int lightBrightnessIndex;

uint8_t getGammaCorrectedBrightness(int brightnessIndex) {
  uint8_t gammaCorrectedBrightness = pgm_read_byte(&gamma[lightBrightnessIndex]);
  return gammaCorrectedBrightness;
}

void turnLightsOn() {
  lightState = LIGHT_STATE_ON;
  //Serial.println("Turning lights on");
}

void turnLightsOff() {
  lightState = LIGHT_STATE_OFF;
  //Serial.println("Turning lights off");
}

void setupLights() {
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);
  lightState = LIGHT_STATE_OFF;
}

unsigned long lightsAnimationTimeout = 0;
void loopLights() {
  unsigned long currentTime = millis();
  if (currentTime > lightsAnimationTimeout) {
    lightsAnimationTimeout = currentTime + LIGHTS_ANIMATION_TIMEOUT_MS;
    
    if (lightState == LIGHT_STATE_ON) {
      lightBrightnessIndex += LIGHT_ON_TRANSITION_STEP;
      lightBrightnessIndex = min(lightBrightnessIndex, lightBrightnessMax);
    } else {
      lightBrightnessIndex -= LIGHT_OFF_TRANSITION_STEP;
      lightBrightnessIndex = max(lightBrightnessIndex, LIGHT_BRIGHTNESS_MIN);
    }

    int gammaCorrectedBrightness = getGammaCorrectedBrightness(lightBrightnessIndex);

    //Serial.print("brightness: ");
    //Serial.println(gammaCorrectedBrightness);
  }
  
  int gammaCorrectedBrightness = getGammaCorrectedBrightness(lightBrightnessIndex);

  analogWrite(ledPin, gammaCorrectedBrightness);

  
}

