/*
  AudioSampler.h - Library for recording microphone input
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.
*/

#ifndef AudioSampler_h
#define AudioSampler_h

#include "Arduino.h"

class AudioSampler
{
  public:
    AudioSampler(int micPin);
    ~AudioSampler();
    void begin(unsigned int sampleRate_hz, unsigned int sampleLength, void (*callback)(unsigned int*, unsigned int));
    void end();
  private:
    unsigned int _micPin = 0;
    unsigned int _sampleRate_hz = 9000;
    unsigned int _sampleRate_interval_microseconds = 0;
    static const unsigned int _signalMax = 0;
    static const unsigned int _signalMin = 1024;
    
    boolean _RUNNING = false;
    const int ANALOG_READ_RESOLUTION = 10; // Bits of resolution for the ADC.
    const int ANALOG_READ_AVERAGING = 16;  // Number of samples to average with each ADC reading.
    
    unsigned int _sampleLength = 256;
    int _carat = 0;
    
    /**
     * this function converts audio volume into 10log10(P1/P2) decibels. 
     * Humans hear small ranges in low volume well, but large changes in loud volume poorly
     */
    static float sampleRangeToDecibels(int sampleRange);
};

#endif

