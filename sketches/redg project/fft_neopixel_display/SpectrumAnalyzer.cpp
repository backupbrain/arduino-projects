/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#include "SpectrumAnalyzer.h"

SpectrumAnalyzer::SpectrumAnalyzer(unsigned int fftResolution, unsigned int sampleRate_hz) {
  _fftResolution = fftResolution;
  _sampleRate_hz = sampleRate_hz;
}


SpectrumAnalyzer::~SpectrumAnalyzer() {
}

float* SpectrumAnalyzer::analyzeWaveform(unsigned int* sampleValues, unsigned int sampleLength) {
    // process incoming sample to include zeroed out "imaginary numbers" for FFT 
    //float fftCompatibleSamples[sampleLength*2];
    float *fftCompatibleSamples;
    fftCompatibleSamples = (float*)malloc((sampleLength*2)*sizeof(float));
    for (int i=0; i<sampleLength; i++) {
      fftCompatibleSamples[i*2] = sampleValues[i];
      fftCompatibleSamples[(i*2)+1] = 0;
    }

    float* frequencyMagnitudes = _getFrequencyMagnitudes(fftCompatibleSamples);
  
    free(fftCompatibleSamples);
    fftCompatibleSamples = NULL;
	
	return frequencyMagnitudes;
}


float* SpectrumAnalyzer::_getFrequencyMagnitudes(float* samples) {
  float frequencyMagnitudes[_fftResolution];
    
  // Run FFT on sample data.
  arm_cfft_radix4_instance_f32 fft_inst;
  arm_cfft_radix4_init_f32(&fft_inst, _fftResolution, 0, 1);
  arm_cfft_radix4_f32(&fft_inst, samples);
  // Calculate magnitude of complex numbers output by the FFT.
  arm_cmplx_mag_f32(samples, frequencyMagnitudes, _fftResolution);
  
  return frequencyMagnitudes;
  
}





float* SpectrumAnalyzer::reduce(float *frequencyMagnitudes, unsigned int desiredResolution) {  
  
  float frequencyWindow[desiredResolution+1];
  float windowSize = (_sampleRate_hz / 2.0) / float(desiredResolution);
  for (int i = 0; i < desiredResolution+1; ++i) {
    frequencyWindow[i] = i*windowSize;
  }
  
  //float spectrum[desiredResolution];
  
  float *spectrum;
 spectrum = (float*)malloc(desiredResolution*sizeof(float));
  
  float intensity, otherMean;
  for (int i = 0; i < desiredResolution; ++i) {
    _windowMean(frequencyMagnitudes,
               _frequencyToBin(frequencyWindow[i]),
               _frequencyToBin(frequencyWindow[i+1]),
               &intensity,
               &otherMean);
    // Convert intensity to decibels.
    intensity = _toDecibels(intensity);
    /*
    // Scale the intensity and clamp between 0 and 1.0.
    intensity -= SPECTRUM_MIN_DB;
    intensity = intensity < 0.0 ? 0.0 : intensity;
    intensity /= (SPECTRUM_MAX_DB-SPECTRUM_MIN_DB);
    intensity = intensity > 1.0 ? 1.0 : intensity;
    */
    
    
    spectrum[i] = intensity;
 //   spectrum[i] = 20;
    
    /*
    Serial.print(i);
    Serial.print(":");
    Serial.print(spectrum[i]);
    Serial.print(",");
    */
    
  }
  /*
  Serial.println("");
  */
  
  return spectrum;
  
}






float SpectrumAnalyzer::_toDecibels(float value) {
  float decibelValue = 20.0*log10(value);
  return decibelValue;
}

// Convert a frequency to the appropriate FFT bin it will fall within.
int SpectrumAnalyzer::_frequencyToBin(float frequency) {
  float binFrequency = float(_sampleRate_hz) / float(_fftResolution);
  return int(frequency / binFrequency);
}



// Compute the average magnitude of a target frequency window vs. all other frequencies.`
void SpectrumAnalyzer::_windowMean(float* magnitudes, int lowBin, int highBin, float* windowMean, float* otherMean) {
    *windowMean = 0;
    *otherMean = 0;
    // Notice the first magnitude bin is skipped because it represents the
    // average power of the signal.
    for (int i = 1; i < _fftResolution/2; ++i) {
      if (i >= lowBin && i <= highBin) {
        *windowMean += magnitudes[i];
      }
      else {
        *otherMean += magnitudes[i];
      }
    }
    *windowMean /= (highBin - lowBin) + 1;
    *otherMean /= (_fftResolution / 2 - (highBin - lowBin));
    
}

