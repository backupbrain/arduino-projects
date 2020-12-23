/**
 * Heartbeat.cpp - Animates a blinking LED to show the Arduino is working
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */
 
#include "Heartbeat.h"

/**
 * Create a Heartbeat (LED Blink) controller
 * 
 * @param heardbeatLedPin the pin that the heartbeat LED is connected to
 */
Heartbeat::Heartbeat(const unsigned int heartbeatLedPin) : _heartbeatLedPin(heartbeatLedPin) {
  pinMode(_heartbeatLedPin, OUTPUT);
}

Heartbeat::~Heartbeat() {
}

/**
 * Beat (Blink) the Heartbeat LED
 */
void Heartbeat::beat() {
  digitalWrite(_heartbeatLedPin, !digitalRead(_heartbeatLedPin));
  Serial.print("Blink: ");
  Serial.println(digitalRead(_heartbeatLedPin));
}

