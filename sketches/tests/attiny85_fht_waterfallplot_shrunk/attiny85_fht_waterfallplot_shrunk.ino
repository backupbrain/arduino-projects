/*
fixFFT http://forum.arduino.cc/index.php/topic,38153.0.html
 this should give an fft with 
 sampling rate:           1ms
 frequency resolution:  500Hz
 lowest frequency:       7.8Hz
 */
 
#include "Simple_Neopixel.h"
#include "Fixed_FFT.h"
#include "WaterfallPlotter.h"


////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE CONSTANTS
// Change these to change how the program records and processes data
////////////////////////////////////////////////////////////////////////////////

/** 
 * System Configuration
 */
#define POWER_LED_PIN = 13

/**
 * Audio Sampling Configuration
 */
#define AUDIO_SAMPLE_RATE_HZ 44100            // Sample rate of the audio in hertz.
#define AUDIO_SAMPLE_LENGTH 64 // was 1024
#define AUDIO_INPUT_PIN A1        // Input ADC pin for audio data.
#define MIN_SOUND_VOLUME 0

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
#define historyLength_seconds  0.25
#define HISTORY_BUFFER_LENGTH 128 //(int)floor(historyLength_seconds*(AUDIO_SAMPLE_RATE_HZ/AUDIO_SAMPLE_LENGTH));

/**
 * Audio Processing Configuration
 */
#define FFT_RESOLUTION 32 // FHT_N/2              // Size of the FFT.  Realistically can only be at most 256 

/**
 * Waterfall Plotter Configuration
 */
#define NEOPIXEL_WIDTH 8 //30; // 8
#define NEOPIXEL_HEIGHT 8 //10; // 8

/**
 * Animation Configuration
 */
#define animationFrameTimeout_us 50000
//const unsigned int animationFrameTimeout_ms = 0;


// Arduino micro
#define NEOPIXEL_PIN 0
/* */


////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

unsigned long lastAnimationFrameUpdateTime;

unsigned int sample;
int samples[AUDIO_SAMPLE_LENGTH];

byte fht_values[FFT_RESOLUTION];
byte fht_history[NEOPIXEL_WIDTH][NEOPIXEL_HEIGHT]; 

WaterfallPlotter waterfallPlotter = WaterfallPlotter(NEOPIXEL_WIDTH, NEOPIXEL_HEIGHT, NEOPIXEL_PIN);



/**
 * boot the chip
 */
void setup() {
  //pinMode(POWER_LED_PIN, OUTPUT);
  //digitalWrite(POWER_LED_PIN, LOW);
  
  pinMode(AUDIO_INPUT_PIN, INPUT);
  //waterfallPlotter.setMaxValue(MAX_BRIGHTNESS);
}
 
 
 


/** 
 * this function is returned every AUDIO_SAMPLE_LENGTH/SAMPLE_RATE_HZ seconds.
 * it takes in discrete audio sample values taken every 1/SAMPLE_RATE_HZ seconds over AUDIO_SAMPLE_LENGTH 
 * it analyzes the waveform for frequency magnitudes, stores the values in a history, then displays into a pixel output
 */
void loop() {
  unsigned long currentTime = micros();
  
   unsigned int sampleIndex = 0;
   unsigned long lastSampleTime = 0;  // Start of sample window
   
   unsigned int _sampleRate_interval_microseconds = 1000000 / AUDIO_SAMPLE_RATE_HZ;
   
  if (currentTime - lastAnimationFrameUpdateTime > animationFrameTimeout_us) {
    
    
    
// get audio samples
  while(sampleIndex < AUDIO_SAMPLE_LENGTH) {
    if (currentTime - lastSampleTime > _sampleRate_interval_microseconds) {
      samples[sampleIndex] = analogRead(AUDIO_INPUT_PIN); // get adc value
      lastSampleTime = currentTime;
      sampleIndex++;
    }
  }
  
  
//    byte pixelValue = audioSampler.getIntensity(samples)/4;
    
    //analyzeFrequencies(samples);
    
  char fht_input[FFT_RESOLUTION];
  for (unsigned int sample = 0; sample < AUDIO_SAMPLE_LENGTH; sample++) {
    if (sample < FHT_N) { // fht_input is FHT_N bytes long
      fht_input[sample] = samples[sample];
    }
  }
  
  //this could be done with the fix_fftr function without the im array.
  Fixed_FFT::fftr(fht_input,7,0);
    // I am only interessted in the absolute value of the transformation
  for (int x=0; x < FFT_RESOLUTION; x++) { //real val is for the amplitude 
    fht_values[x] = sqrt(fht_input[x] * fht_input[x]); // create real value from possible imaginary ones
  }
    

    
    
    
  // push everything in the fht data array over by one  
  for (int x=NEOPIXEL_WIDTH-1; x>0; x--) {
    for (int y=0; y<NEOPIXEL_HEIGHT; y++) {
      fht_history[x][y] =  fht_history[x-1][y];
    }
  }	
  
  
    // get rid of the "volume metric" on the fht
    // add new fht data to the history
    for (int x=0; x < FFT_RESOLUTION; x++) {
      fht_history[x][0] = fht_values[x+2];
      if (fht_history[x][0] < MIN_SOUND_VOLUME) {
        fht_history[x][0] = 0;
      } else {
        fht_history[x][0] = fht_history[x][0] - MIN_SOUND_VOLUME;
      }
    }

    waterfallPlotter.update(fht_history[0]);
    waterfallPlotter.display();
    
    lastAnimationFrameUpdateTime = currentTime;
  }

}
