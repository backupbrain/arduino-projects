#include <Adafruit_NeoPixel.h>
#include "AudioSampler.h"
#include "FHTHistory.h"
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
const unsigned int AUDIO_SAMPLE_RATE_HZ = 44100;             // Sample rate of the audio in hertz.
const unsigned int AUDIO_SAMPLE_LENGTH = 128; // was 1024
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
 * FHTHistory Configuration
 */
// 128 of 44100 samples per second = 2.9 milliseconds.  
// roughly 340 samples per second.  
// so if we want 1 second of history, then we want 340 samples
// HISTORY_BUFFER_LENGTH = sample_length_seconds * 340
// WARNING: an arduino may only have 1 or 2 kb or so of RAM to work with
float historyLength_seconds = 0.25;
const unsigned int HISTORY_BUFFER_LENGTH = 340; //(int)floor(historyLength_seconds*(AUDIO_SAMPLE_RATE_HZ/AUDIO_SAMPLE_LENGTH));



/**
 * Audio Processing Configuration
 */
const unsigned int SPECTRUM_MIN_DB = 30;
const unsigned int SPECTRUM_MAX_DB = 90;
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
//const uint8_t NUM_NEOPIXEL_PINS =  5; /* 5 */;
//uint8_t NEOPIXEL_PINS[] = {/*3, 5, 6, /* */ 9, 10, 11, 12, 13};
//const uint8_t NUM_NEOPIXEL_PINS =  3; 
//uint8_t NEOPIXEL_PINS[] = { 10, 11, 12 };
const uint8_t NEOPIXEL_PIN =  9; 
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

Adafruit_NeoPixel pixelStrip = Adafruit_NeoPixel(NEOPIXEL_WIDTH*NEOPIXEL_HEIGHT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);


/**
 * boot the chip
 */
void setup() {
  //Serial.begin(115200);
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, LOW);
  
  waterfallPlotter.setMaxValue(MAX_BRIGHTNESS);
  
  pixelStrip.begin();
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
    //analogWrite(POWER_LED_PIN, pixelValue);  // diagnostic led
    
    analyzeFrequencies(samples);

    // get rid of the "volume metric on the fht
    for (int x=0; x < FFT_RESOLUTION; x++) {
      fht_values[x] = fht_log_out[x+2];
      if (fht_values[x] < MIN_SOUND_VOLUME) {
        fht_values[x] = 0;
      } else {
        fht_values[x] = fht_values[x] - MIN_SOUND_VOLUME;
      }
    }
    
    
    
  
    for (int pixel = 0; pixel < 8; pixel++) {
      //pixelValue = samples[pixel]/32;
      pixelValue = fht_values[pixel]/32;
      pixelStrip.setPixelColor(pixel, pixelValue, pixelValue, pixelValue);
    
    }
    //pixelStrip.setPixelColor(0, pixelValue, pixelValue, pixelValue);
    pixelStrip.show();
    
    /*
    for (int x=0; x < FHT_N/2; x++) {
      Serial.print(fht_log_out[x]);
      Serial.print(",");
    }
    Serial.println();
    /* */
  
    //fhtHistory.loadFromFHT(fht_values);
  
    //Serial.print("freeMemory()=");
    //Serial.println(freeMemory());
  
  
    //waterfallPlotter.update(fhtHistory.getData());
    //waterfallPlotter.display();
  
  
  
    //Serial.print("freeMemory()=");
    //Serial.println(freeMemory());
    
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
