/*
fixFFT http://forum.arduino.cc/index.php/topic,38153.0.html
 this should give an fft with 
 sampling rate:           1ms
 frequency resolution:  500Hz
 lowest frequency:       7.8Hz
 */
 
#include "Simple_Neopixel.h"
#include "AudioSampler.h"
#include "Fixed_FFT.h"
#include "FHTHistory.h"
#include "WaterfallPlotter.h"


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
const unsigned int AUDIO_SAMPLE_RATE_HZ = 44100;             // Sample rate of the audio in hertz.
const unsigned int AUDIO_SAMPLE_LENGTH = 64; // was 1024
const byte AUDIO_INPUT_PIN = A1;        // Input ADC pin for audio data.

/** 
 * FHT library Configuration for arduino
 */
#define FHT_N 64 // set to 256 point fht, will initialize: int fht_input[FHT_N];

/**
 * FHTHistory Configuration
 */
// 128 of 44100 samples per second = 2.9 milliseconds.  
// roughly 340 samples per second.  
// so if we want 1 second of history, then we want 340 samples
// HISTORY_BUFFER_LENGTH = sample_length_seconds * 340
// WARNING: an arduino may only have 1 or 2 kb or so of RAM to work with
float historyLength_seconds = 0.25;
const unsigned int HISTORY_BUFFER_LENGTH = 128; //(int)floor(historyLength_seconds*(AUDIO_SAMPLE_RATE_HZ/AUDIO_SAMPLE_LENGTH));

/**
 * Audio Processing Configuration
 */
const unsigned int MIN_SOUND_VOLUME = 0;
const unsigned int FFT_RESOLUTION = FHT_N/2;              // Size of the FFT.  Realistically can only be at most 256 

/**
 * Waterfall Plotter Configuration
 */
const unsigned int NEOPIXEL_WIDTH = 8; //30; // 8
const unsigned int NEOPIXEL_HEIGHT = 8; //10; // 8
const float MAX_BRIGHTNESS = 1.0;

/**
 * Animation Configuration
 */
const unsigned int animationFrameTimeout_ms = 50;
//const unsigned int animationFrameTimeout_ms = 0;


// Arduino micro
const uint8_t NEOPIXEL_PIN =  6; 
/* */


////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

unsigned long lastAnimationFrameUpdateTime;

unsigned int sample;
int samples[AUDIO_SAMPLE_LENGTH];

byte fht_values[FFT_RESOLUTION]; 

AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN, AUDIO_SAMPLE_RATE_HZ, AUDIO_SAMPLE_LENGTH); // sample input from the microphone
FHTHistory fhtHistory = FHTHistory(NEOPIXEL_WIDTH, NEOPIXEL_HEIGHT, FFT_RESOLUTION);
WaterfallPlotter waterfallPlotter = WaterfallPlotter(NEOPIXEL_WIDTH, NEOPIXEL_HEIGHT, NEOPIXEL_PIN, deadmoau5_8x8_bits); 



/**
 * boot the chip
 */
void setup() {
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, LOW);
  
  waterfallPlotter.setMaxValue(MAX_BRIGHTNESS);
}
 
 
 


/** 
 * this function is returned every AUDIO_SAMPLE_LENGTH/SAMPLE_RATE_HZ seconds.
 * it takes in discrete audio sample values taken every 1/SAMPLE_RATE_HZ seconds over AUDIO_SAMPLE_LENGTH 
 * it analyzes the waveform for frequency magnitudes, stores the values in a history, then displays into a pixel output
 */
void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastAnimationFrameUpdateTime > animationFrameTimeout_ms) {
    audioSampler.sample(samples);
    
    byte pixelValue = audioSampler.getIntensity(samples)/4;
    
    analyzeFrequencies(samples);
    

    // get rid of the "volume metric" on the fht
    for (int x=0; x < FFT_RESOLUTION; x++) {
      fht_values[x] = fht_values[x+2];
      if (fht_values[x] < MIN_SOUND_VOLUME) {
        fht_values[x] = 0;
      } else {
        fht_values[x] = fht_values[x] - MIN_SOUND_VOLUME;
      }
    }
    
  
    fhtHistory.loadFromFHT(fht_values);

    waterfallPlotter.update(fhtHistory.getData());
    waterfallPlotter.display();
    
    lastAnimationFrameUpdateTime = currentTime;
  }

}


byte* analyzeFrequencies(int* samples) {
  
  char fht_input[FFT_RESOLUTION];
  for (unsigned int sample = 0; sample < AUDIO_SAMPLE_LENGTH; sample++) {
    if (sample < FHT_N) { // fht_input is FHT_N bytes long
      fht_input[sample] = samples[sample];
    }
  }
  
  
  
  //this could be done with the fix_fftr function without the im array.
  Fixed_FFT::fftr(fht_input,7,0);
    /*
     Performs foward or inverse Fourier transform.
     //fix_fft (char fr[], char fi[], int m, int inverse)
     //fr is a real data set,
     //fi is the imaginary data set,
     // m is log2(n) where n is number of data points (log2 (128) = 7)
     //0 is set for forward transform. 1 would be inverse transform. Apparently inverse does not work,
     */
    // I am only interessted in the absolute value of the transformation
  for (int x=0; x < FFT_RESOLUTION; x++) { //real val is for the amplitude 
    fht_values[x] = sqrt(fht_input[x] * fht_input[x]); 
  }
}
 
