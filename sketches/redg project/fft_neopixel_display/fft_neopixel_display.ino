#include <Adafruit_NeoPixel.h>
#include "AudioSampler.h"
#include "SpectrumAnalyzer.h"
#include "FrequencyHistory.h"
#include "WaterfallPlotter.h"
#include <MemoryFree.h>



////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE CONSTANTS
// Change these to change how the program records and processes data
////////////////////////////////////////////////////////////////////////////////

/** 
 * System Configuration
 */
const byte POWER_LED_PIN = 13;

/**
 * Audio Sampling Configuration
 */
const unsigned int SAMPLE_RATE_HZ = 44100;             // Sample rate of the audio in hertz.
const unsigned int AUDIO_SAMPLE_LENGTH = 4096;
const byte AUDIO_INPUT_PIN = A0;        // Input ADC pin for audio data.

float historyLength_seconds = 0.25;
const unsigned int HISTORY_LENGTH = (int)floor(1000*historyLength_seconds)*(SAMPLE_RATE_HZ/AUDIO_SAMPLE_LENGTH);

/**
 * Audio Processing Configuration
 */
const unsigned int SPECTRUM_MIN_DB = 30;
const unsigned int SPECTRUM_MAX_DB = 90;
const unsigned int FFT_RESOLUTION = 256;              // Size of the FFT.  Realistically can only be at most 256 


/**
 * Waterfall Plotter Configuration
 */
const unsigned int NEOPIXEL_WIDTH = 8;
const unsigned int NEOPIXEL_HEIGHT = 6;

uint8_t NEOPIXEL_PINS[] = {
  0,
  1,
  2,
  3,
  4,
  5
};




////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////


unsigned int sample;


AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN); // sample input from the microphone
SpectrumAnalyzer spectrumAnalyzer = SpectrumAnalyzer(FFT_RESOLUTION, SAMPLE_RATE_HZ); // FFT functionality
FrequencyHistory frequencyHistory = FrequencyHistory(NEOPIXEL_WIDTH, NEOPIXEL_HEIGHT, FFT_RESOLUTION);
WaterfallPlotter waterfallPlotter = WaterfallPlotter(NEOPIXEL_WIDTH, NEOPIXEL_HEIGHT, NEOPIXEL_PINS); 


/**
 * boot the chip
 */
void setup() {
  Serial.begin(9600);
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH);
  
  // this is the last thing we do before taking over the CPU
  audioSampler.begin(SAMPLE_RATE_HZ, AUDIO_SAMPLE_LENGTH, audioSampleCallback);
}


void loop() {

}

/** 
 * this function is returned every AUDIO_SAMPLE_LENGTH/SAMPLE_RATE_HZ seconds.
 * it takes in discrete audio sample values taken every 1/SAMPLE_RATE_HZ seconds over AUDIO_SAMPLE_LENGTH 
 * it analyzes the waveform for frequency magnitudes, stores the values in a history, then displays into a pixel output
 */
 
void audioSampleCallback(unsigned int* sampleValues, unsigned int sampleLength) {
  //Serial.println(sampleValues[0]);
  float* frequencyMagnitudes = spectrumAnalyzer.analyzeWaveform(sampleValues, sampleLength);
  
  frequencyHistory.loadFromFFT(frequencyMagnitudes);
  
  //Serial.print("freeMemory()=");
  //Serial.println(freeMemory());
  
  
  waterfallPlotter.update(frequencyHistory.getData());
  waterfallPlotter.display();
  
  
}
