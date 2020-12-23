/*
  AudioSampler.cpp - Library for recording microphone input
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.
*/
#include "AudioSampler.h"

AudioSampler::AudioSampler(int micPin) {  
  _micPin = micPin;
  
  pinMode(_micPin, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);
}

AudioSampler::~AudioSampler() {
}

void AudioSampler::begin(unsigned int sampleRate_hz, unsigned int sampleLength, void (*callback)(unsigned int*, unsigned int)) {
  
  _sampleRate_hz = sampleRate_hz;
  // set the sample rate interval in milliseconds
  _sampleRate_interval_microseconds = 1000000 / sampleRate_hz;
  
  _sampleLength = sampleLength;
  
  unsigned int *_sampleValues;
 _sampleValues = (unsigned int*)malloc(_sampleLength*sizeof(unsigned int));
   _RUNNING = true;
   _carat = 0;
   unsigned long lastSampleTime = 0;  // Start of sample window
   unsigned long currentTime = 0;
  
   while (_RUNNING) {
     currentTime = micros();
     if (micros() - lastSampleTime > _sampleRate_interval_microseconds) {
       _sampleValues[_carat++] = analogRead(_micPin);
       lastSampleTime = currentTime;
       if (_carat >= _sampleLength) {
         callback(_sampleValues, _sampleLength);
         _carat = 0;
       }
     }
   }
   
   free(_sampleValues);
}

void AudioSampler::end() {
  _RUNNING = false;
}

