/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef BeatAnalyzer_h
#define BeatAnalyzer_h

#include "Arduino.h"

class BeatAnalyzer
{
  public:
    BeatAnalyzer(int sampleRate_hz);
    ~BeatAnalyzer();
    boolean detectBeat(float *fft, int fftSize);
    float* getHistoryBuffer();
    float* getAverageIntensities();
    float getBeatProbability();
    int getHistoryBufferSize();
    int getFrequencyResolution();
  private:
    void _buildSubBands(float *fft, int fftSize);
    void _pushHistory();
    boolean _detectBeat();
    float _toDecibels(float value);
    int _frequencyToBin(float frequency);
    void _windowMean(float* magnitudes, int lowBin, int highBin, float* windowMean, float* otherMean);
    static const int _frequencyResolution = 32; //32;
    static const int _historyBufferSize = 32; //43;
    float _historyBuffer[_historyBufferSize][_frequencyResolution];
    float _averageIntensities[_frequencyResolution];
    float _beatProbability = 0; // 0-1
    
    float _frequencyWindow[_frequencyResolution+1];
    float _beatThreshold = 1.5; 
    int _sampleRate_hz = 9000;
};

#endif

