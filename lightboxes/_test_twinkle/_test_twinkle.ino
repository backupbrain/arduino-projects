/**
 * System
 */

#include "TwinkleLights.h"
#include "GammaCorrect.h"

const unsigned int LED_COUNT = 1; // number of LED pins, 144
const byte LED_PIN = 6; 

/**
 * Animation configuration
 */
const unsigned int FRAMERATE_MS = 16; // 16 ms = 60 fps


/**
 * Internal state
 */
long frameStartTime_ms;
TwinkleLights twinkle = TwinkleLights(LED_COUNT);


void setup() {
  pinMode(LED_PIN, OUTPUT);
  //Serial.begin(9600);
  randomSeed(analogRead(0));
  frameStartTime_ms = millis();
}


void loop() {
  long timeDelay = millis() - frameStartTime_ms;
  if (timeDelay > FRAMERATE_MS) {
    unsigned int *brightnesses = twinkle.render();
    analogWrite(LED_PIN, gammaCorrect(brightnesses[0]));
    //Serial.println(pixels[0].brightness);
    
    frameStartTime_ms = millis();
  }  
}
