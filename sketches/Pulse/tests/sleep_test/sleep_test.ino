/*******
 * 
 * https://www.youtube.com/watch?v=ktvUunBQQD0&spfreload=10
 */

#include <avr/sleep.h>

const unsigned int indicatorLED = 13;
const unsigned int interruptID = 0;
const unsigned int blinkDelay_ms = 900;
const unsigned int numBlinks = 4;

int count = 0;

void setup() {
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  pinMode(indicatorLED, OUTPUT);
}


void loop() {
  if (count < blinkDelay_ms) {
    digitalWrite(indicatorLED, HIGH);
    delay(blinkDelay_ms);
    digitalWrite(indicatorLED, LOW);
    delay(blinkDelay_ms);
    count++;
  } else {
    attachInterrupt(interruptID, interruptFunction, HIGH);
    sleep_cpu();
    count = 0;
  }
}


void interruptFunction() {
  detachInterrupt(0);
  sleep_disable();
}
 
