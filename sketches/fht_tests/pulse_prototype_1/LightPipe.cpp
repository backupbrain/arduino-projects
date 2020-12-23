/*
  LightPipe.h - A library for converting beats into lights
  Created by Tony Gaitatzis, May 31, 2015.
  Released into the public domain.
*/
#include "LightPipe.h"

LightPipe::LightPipe(byte outputPin) {
  this->_outputPin = outputPin;
  pinMode(this->_outputPin, OUTPUT);
  
  this->_minProbabilityToTriggerBeatTimeout = 75;
  this->_minBeatTimeout_ms = 100;
  this->_brightessTimeout_ms = 250;
}

LightPipe::~LightPipe() {
}


void LightPipe::react(byte beatProbability) {
  boolean updateBeat = false;
  
  unsigned long currentTime = millis();
  
  // if the beatProbability exceeds the current led brightess...
  if (beatProbability >= this->_currentBrightness) {
    // if we detected a beat...
    if ((currentTime - this->_lastBeatDetectedTime) > this->_minBeatTimeout_ms) {
      updateBeat = true;
    }
    // if we are triggering a beat timeout...
    if (beatProbability > this->_minProbabilityToTriggerBeatTimeout) {
      this->_lastBeatDetectedTime = currentTime;
    }
  }
      
      
  this->update(updateBeat, beatProbability);
}

void LightPipe::update(boolean updateBeat, unsigned int beatProbability) {
  unsigned long currentTime = millis();
  if (updateBeat) {
    this->_lastBeatDetectedTime = currentTime;
    this->_currentBrightness = beatProbability;
  } else {
    this->_currentBrightness = this->_currentBrightness * (int)(this->_brightessTimeout_ms - (currentTime - this->_lastBeatDetectedTime))/this->_brightessTimeout_ms; 
  }
}

void LightPipe::display() {
  analogWrite(this->_outputPin, this->_currentBrightness/2);
  //Serial.print("brightness: ");
  //Serial.println(this->_currentBrightness);
}

byte LightPipe::getBrightness() {
  return this->_currentBrightness;
}
