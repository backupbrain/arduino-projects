/*
  BeatAnalyzer.cpp - Library for detecting the beat in music
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#include "BeatAnalyzer.h"

BeatAnalyzer::BeatAnalyzer(int sampleRate_hz) {  
  _sampleRate_hz = sampleRate_hz;
  
  
  // Set the frequency window values by evenly dividing the possible frequency
  // spectrum across the number of neo pixels.
  float windowSize = (_sampleRate_hz / 2.0) / float(_frequencyResolution);
  for (int i = 0; i < _frequencyResolution+1; ++i) {
    _frequencyWindow[i] = i*windowSize;
  }
}

BeatAnalyzer::~BeatAnalyzer() {
}

boolean BeatAnalyzer::detectBeat(float *fft, int fftSize) {
  _pushHistory();
  _buildSubBands(fft, fftSize);
  boolean beatDetected = _detectBeat();
  return beatDetected;
}

void BeatAnalyzer::_pushHistory() {
  for (int historyIndex = _historyBufferSize - 1; historyIndex >= 0; historyIndex--) {
    for (int frequencyIndex = 0; frequencyIndex < _frequencyResolution; frequencyIndex++) {
      _historyBuffer[historyIndex][frequencyIndex] = _historyBuffer[historyIndex-1][frequencyIndex];
    }
  }
}

void BeatAnalyzer::_buildSubBands(float *fft, int fftSize) {  
  // Update each LED based on the intensity of the audio 
  // in the associated frequency window.
  float intensity, otherMean;
  for (int i = 0; i < _frequencyResolution; ++i) {
    _windowMean(fft, 
               _frequencyToBin(_frequencyWindow[i]),
               _frequencyToBin(_frequencyWindow[i+1]),
               &intensity,
               &otherMean);
    // Convert intensity to decibels.
    intensity = _toDecibels(intensity);
    //if (intensity > 200) intensity = 0;
    //if (intensity < 0) intensity = 0;
    _historyBuffer[0][i] = intensity;
    
  }
    
}

float BeatAnalyzer::_toDecibels(float value) {
  float decibelValue = 20.0*log10(value);
  return decibelValue;
}

boolean BeatAnalyzer::_detectBeat() {
  boolean beatDetected = false;
  
  float beatProbabilities[_frequencyResolution];
  
  for (int frequencyIndex = 1; frequencyIndex < _frequencyResolution; frequencyIndex++) {
    for (int historyIndex = 1; historyIndex < _historyBufferSize; historyIndex++) {
      _averageIntensities[frequencyIndex] += _historyBuffer[historyIndex][frequencyIndex];
    }
    _averageIntensities[frequencyIndex] = _averageIntensities[frequencyIndex]/(_historyBufferSize-1);

    //if (_averageIntensities[frequencyIndex] > 200) _averageIntensities[frequencyIndex] = 0;
    //if (_averageIntensities[frequencyIndex] < 0) _averageIntensities[frequencyIndex] = 0;
      
    
    //Serial.print((int)averageIntensities[frequencyIndex]);
    //Serial.print(",");

  }
  //Serial.println("");  
  
  for (int frequencyIndex = 1; frequencyIndex < _frequencyResolution; frequencyIndex++) {
    
    beatProbabilities[frequencyIndex] = _historyBuffer[0][frequencyIndex] - _historyBuffer[4][frequencyIndex];
    
    
    if (_historyBuffer[0][frequencyIndex] > (_historyBuffer[4][frequencyIndex] * _beatThreshold)) {
      
    Serial.print((int)_historyBuffer[0][frequencyIndex] -(int) _historyBuffer[2][frequencyIndex]);
    Serial.print(",");
      beatDetected = true;
    }
  }
  
  _beatProbability = 0;
  for (int frequencyIndex = 1; frequencyIndex < _frequencyResolution; frequencyIndex++) {
    _beatProbability += beatProbabilities[frequencyIndex];
  }
  _beatProbability /= (_frequencyResolution-1);
  
  
  //Serial.println("");  
  //Serial.println("-----------------------"); 
  
  return beatDetected;
}

float* BeatAnalyzer::getHistoryBuffer() {
  return *_historyBuffer;
}


float* BeatAnalyzer::getAverageIntensities() {
  return _averageIntensities;
}

float BeatAnalyzer::getBeatProbability() {
  return _beatProbability;
}

// Convert a frequency to the appropriate FFT bin it will fall within.
int BeatAnalyzer::_frequencyToBin(float frequency) {
  float binFrequency = float(_sampleRate_hz) / float(_frequencyResolution);
  return int(frequency / binFrequency);
}

// Compute the average magnitude of a target frequency window vs. all other frequencies.`
void BeatAnalyzer::_windowMean(float* magnitudes, int lowBin, int highBin, float* windowMean, float* otherMean) {
    *windowMean = 0;
    *otherMean = 0;
    // Notice the first magnitude bin is skipped because it represents the
    // average power of the signal.
    for (int i = 1; i < _frequencyResolution/2; ++i) {
      if (i >= lowBin && i <= highBin) {
        *windowMean += magnitudes[i];
      }
      else {
        *otherMean += magnitudes[i];
      }
    }
    *windowMean /= (highBin - lowBin) + 1;
    *otherMean /= (_frequencyResolution / 2 - (highBin - lowBin));
    
    //Serial.print(*windowMean**windowMean);
}
