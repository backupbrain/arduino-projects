/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef SpectrumAnalyzer_h
#define SpectrumAnalyzer_h

#include "Arduino.h"

#define ARM_MATH_CM4
#include <arm_math.h>

class SpectrumAnalyzer
{
  public:
    SpectrumAnalyzer(unsigned int fftResolution, unsigned int sampleRate_hz);
    ~SpectrumAnalyzer();
    float* analyzeWaveform(unsigned int *sampleValues, unsigned int sampleLength);
    float* reduce(float *frequencyMagnitudes, unsigned int desiredResolution);  

  private:
    void _buildSubBands(float *fft, int fftSize);
    void _pushHistory();
    float* _getFrequencyMagnitudes(float *samples);
    float _toDecibels(float value);
    int _frequencyToBin(float frequency);
    void _windowMean(float* magnitudes, int lowBin, int highBin, float* windowMean, float* otherMean);
    
    
    unsigned int _fftResolution = 256;
    unsigned int _sampleRate_hz = 9000;
};

#endif

