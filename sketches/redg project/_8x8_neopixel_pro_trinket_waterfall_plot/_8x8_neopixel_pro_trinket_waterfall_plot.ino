#include <Adafruit_NeoPixel.h>
#include "AudioSampler.h"
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
const byte AUDIO_INPUT_PIN = A0;        // Input ADC pin for audio data.


/** 
 * FHT library Configuration for arduino
 */
#define LOG_OUT 1 // use the log output function
//#define OCTAVE 1 // use the fft_mag_octave output function
//#define OCT_NORM 0
#define FHT_N 128 // set to 256 point fht, will initialize: int fht_input[FHT_N];
#include <FHT.h>

/**
 * Audio Processing Configuration
 */
const unsigned int MIN_SOUND_VOLUME = 50;
const unsigned int FFT_RESOLUTION = FHT_N/2;              // Size of the FFT.  Realistically can only be at most 256 


/**
 * Waterfall Plotter Configuration
 */
const unsigned int NEOPIXEL_WIDTH = 8; //30; // 8
const unsigned int NEOPIXEL_HEIGHT = 8; //10; // 8
const float MAX_BRIGHTNESS = 1.0;
const uint8_t NEOPIXEL_PIN =  6; 

/**
 * Animation Configuration
 */
const unsigned int animationFrameTimeout_ms = 75;


////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

unsigned long lastAnimationFrameUpdateTime;

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
  
  //waterfallPlotter.setMaxValue(MAX_BRIGHTNESS);
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
    
    byte normalizedVolume = audioSampler.getIntensity(samples)/16;
    
    analyzeFrequencies(samples);
  

    // get rid of the "volume" metric on the fht
    for (int x=0; x < FFT_RESOLUTION; x++) {
      //fht_values[x] = fht_oct_out[x+2];
      fht_values[x] = fht_log_out[x+2];
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
  for (unsigned int sample = 0; sample < AUDIO_SAMPLE_LENGTH; sample++) {
    if (sample < FHT_N) { // fht_input is FHT_N bytes long
      fht_input[sample] = samples[sample];
    }
  }
  // process data
  fht_window(); // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  //fht_mag_octave(); // take the output of the fht, save into fht_oct_out[]
  // this will result in log(N) bins
  // apparently the input is takes from fft_input?
   
  // alternatevly, can use fht_mag_lin, which returns fht_lin_out[] linearly
   
  //fht_mag_lin();
  fht_mag_log(); // take the output of the fht
  //fht_mag_octave();
   
  // fht creates and populates fht_log_out
  return fht_log_out;
  //return fht_oct_out;
}

