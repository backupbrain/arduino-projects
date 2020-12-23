
#include <Adafruit_NeoPixel.h>

#include "AudioSampler.h"
#include "SpectrumAnalyzer.h"
#include "PixelEqualizer.h"

int SAMPLE_RATE_HZ = 5000;             // Sample rate of the audio in hertz.

const int AUDIO_INPUT_PIN = 15  ;        // Input ADC pin for audio data.

const int FFT_RESOLUTION = 256;              // Size of the FFT.  Realistically can only be at most 256 
const int AUDIO_SAMPLE_LENGTH = 256;
const int NEOPIXEL_WIDTH = 8;
const int NEOPIXEL_HEIGHT = 8;

const int NEOPIXEL_PIN = 3;

////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

float *frequencyMagnitudes;
float *spectrum;


AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN); // sample input from the microphone
SpectrumAnalyzer spectrumAnalyzer = SpectrumAnalyzer(FFT_RESOLUTION, SAMPLE_RATE_HZ);

PixelEqualizer equalizer = PixelEqualizer(NEOPIXEL_PIN, NEOPIXEL_WIDTH, NEOPIXEL_HEIGHT);

void setup() {
  Serial.begin(9600);  
  
  audioSampler.begin(SAMPLE_RATE_HZ, AUDIO_SAMPLE_LENGTH, audioSampleCallback);
  
}


void loop() {

}

void audioSampleCallback(unsigned int* sampleValues, unsigned int sampleLength) {
  // process incoming sample to include zeroed out "imaginary numbers" for FFT 
  //float fftCompatibleSamples[sampleLength*2];
  float *fftCompatibleSamples;
  fftCompatibleSamples = (float*)malloc((sampleLength*2)*sizeof(float));
  for (int i=0; i<sampleLength; i++) {
    fftCompatibleSamples[i*2] = sampleValues[i];
    fftCompatibleSamples[(i*2)+1] = 0;
  }
  frequencyMagnitudes = spectrumAnalyzer.analyzeWaveform(fftCompatibleSamples);
  
  free(fftCompatibleSamples);
  
  spectrum = spectrumAnalyzer.reduce(frequencyMagnitudes, NEOPIXEL_WIDTH);
  
  /*
  for (int i=0; i<NEOPIXEL_WIDTH; i++) {
    Serial.print(spectrum[i]);
    Serial.print(",");
  }
  Serial.println("\n");
  */
  
  equalizer.display(spectrum);
  
}

