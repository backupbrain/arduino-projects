/*
  AudioSampler.h - Library for recording microphone input
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.
  
  Usage:
  const unsigned int AUDIO_SAMPLE_RATE_HZ = 44100;  // Sample rate of the audio in hertz.
  const unsigned int AUDIO_SAMPLE_LENGTH = 4096;
  const byte AUDIO_INPUT_PIN = A0;        // Input ADC pin for audio data.
  
  unsigned int samples[256];
  
  AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN, AUDIO_SAMPLE_RATE_HZ, AUDIO_SAMPLE_LENGTH);
  audioSampler.sample(samples);
  
*/

#ifndef AudioSampler_h
#define AudioSampler_h

#include "Arduino.h"

class AudioSampler
{
  public:
    AudioSampler(byte micPin, unsigned int sampleRate_hz, unsigned int sampleLength);
    ~AudioSampler();
    void sample(char* sampleValues);
    unsigned int getIntensity(char* sampleValues);
  private:
    byte _micPin; // = 0
    unsigned int _sampleRate_hz; // = 9000;
    unsigned int _sampleRate_interval_microseconds; // = 0;
    static const unsigned int _signalMax = 0;
    static const unsigned int _signalMin = 1024;
    
    static const int ANALOG_READ_RESOLUTION = 10; // Bits of resolution for the ADC.
    static const int ANALOG_READ_AVERAGING = 16;  // Number of samples to average with each ADC reading.
    
    unsigned int _sampleLength;//  = 256;
    
    void _initialize();
    /**
     * this function converts audio volume into 10log10(P1/P2) decibels. 
     * Humans hear small ranges in low volume well, but large changes in loud volume poorly
     */
    static float sampleRangeToDecibels(int sampleRange);
};

#endif

