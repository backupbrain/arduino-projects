#include <avr/sleep.h>

const int indicatorPin = 13;
const int interruptID = 0; // pin 3 on Arduino Micro
volatile int state = LOW;

void setup() {
  pinMode(indicatorPin, OUTPUT);
  attachInterrupt(interruptID, handleInterrupt, FALLING);

  digitalWrite(indicatorPin, HIGH);
  Serial.begin(9600);
  Serial.println("Interrupt test");
}

void loop() {
  /*
  digitalWrite(indicatorPin, state);
  
  if (state == HIGH) {
    Serial.println("Setting state to HIGH");
  } else {
    Serial.println("Setting state to LOW");
  }
  /* */
  delay(1000);
  sleep();
 
}

void sleep() {
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  attachInterrupt(interruptID, handleInterrupt, LOW);
  digitalWrite(indicatorPin, LOW);
  sleep_mode();

  //sleep_disable();
  //digitalWrite(indicatorPin, HIGH);
}

void handleInterrupt() {
  noInterrupts();
  detachInterrupt(interruptID);
  sleep_disable();
  digitalWrite(indicatorPin, HIGH);
  state = !state;
  interrupts();
}
