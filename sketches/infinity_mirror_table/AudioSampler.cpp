/*
  AudioSampler.cpp - Library for recording microphone input
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.
*/
#include "AudioSampler.h"

AudioSampler::AudioSampler(byte micPin, unsigned int sampleRate_hz, unsigned int sampleLength) {
  this->_micPin = micPin;
  
  pinMode(this->_micPin, INPUT);
  //analogReadResolution(ANALOG_READ_RESOLUTION);
  //analogReadAveraging(ANALOG_READ_AVERAGING);
  
  this->_sampleRate_hz = sampleRate_hz;
  // set the sample rate interval in milliseconds
  this->_sampleRate_interval_microseconds = 1000000 / sampleRate_hz;
  this->_sampleLength = sampleLength;
}

AudioSampler::~AudioSampler() {
}


/**
 * sample a number of times, record values into samples, and quit
 *
 * usage: audioSampler.sample(sampleValues);
 * modifies sampleValues
 * returns void
 */
void AudioSampler::sample(int* sampleValues) {
   unsigned int sampleIndex = 0;
   unsigned long lastSampleTime = 0;  // Start of sample window
   unsigned long currentTime = 0;

  while(sampleIndex < this->_sampleLength) {
    currentTime = micros();
    if (currentTime - lastSampleTime > this->_sampleRate_interval_microseconds) {
      sampleValues[sampleIndex] = analogRead(this->_micPin); // get adc value
      lastSampleTime = currentTime;
      sampleIndex++;
    }
  }
}

unsigned int AudioSampler::getIntensity(int* sampleValues) {
  
    unsigned int intensity = 0; // peak-to-peak level
    
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;
       
    for (int i = 0; i < this->_sampleLength; i++) {
      if (sampleValues[i] < 1024) {
        if (sampleValues[i] > signalMax) {
          signalMax = sampleValues[i]; // save just the max levels
        } else {
          signalMin = sampleValues[i]; // save just the min levels
        }
      } 
    }
    
    intensity = signalMax - signalMin; // max - min = peak-peak amplitude
    
    return intensity;
}

/**
 * this function converts audio volume into 10log10(P1/P2) decibels. 
 * Humans hear small ranges in low volume well, but large changes in loud volume poorly
 */
float AudioSampler::sampleRangeToDecibels(int sampleRange) {
  float decibels = 20*log10((float)sampleRange);
  return decibels;
}

