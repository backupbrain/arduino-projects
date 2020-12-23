/**
   heartbeat.ino
*/

#include "Heartbeat.h"
#include "gammaCorrect.h"

static const unsigned long animationDelay_ms = 33;
unsigned long lastAnimationTime_ms;

static const unsigned int heartbeatLedPin = 6;
static const unsigned int numLeds = 15;

Heartbeat heartbeat = Heartbeat();

void setup() {
  Serial.begin(9600);
  pinMode(heartbeatLedPin, OUTPUT);
}

void loop() {
  unsigned long currentTime_ms = millis();
  if (currentTime_ms - lastAnimationTime_ms > animationDelay_ms) {
    unsigned char brightness = heartbeat.render();
    analogWrite(heartbeatLedPin, gammaCorrect(brightness));
    //printBars(brightness);
    lastAnimationTime_ms = currentTime_ms;
  }
}

void printBars(unsigned int val) {
  for (int index = 0; index < val; index += 4) {
    Serial.print(F("#"));
  }
  Serial.println(F(""));
  
}

