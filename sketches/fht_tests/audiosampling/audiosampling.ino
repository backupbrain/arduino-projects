#include <Adafruit_NeoPixel.h>    
#include "AudioSampler.h"


/** 
 * FHT library Configuration
 */
#define LOG_OUT 1 // use the log output function
#define OCTAVE 1 // use the fft_mag_octave output function
#define FHT_N 256 // set to 256 point fht, will initialize: int fht_input[FHT_N];

#include <FHT.h>


int output = 0;

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
const unsigned int AUDIO_SAMPLE_LENGTH = FHT_N;
const byte AUDIO_INPUT_PIN = A0;        // Input ADC pin for audio data.

float historyLength_seconds = 0.25;
const unsigned int HISTORY_LENGTH = (int)floor(1000*historyLength_seconds)*(AUDIO_SAMPLE_RATE_HZ/AUDIO_SAMPLE_LENGTH);

/**
 * Display Configuration
 */
const byte NUM_NEOPIXELS = 8;
const byte NEOPIXEL_PIN = 9;


////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

unsigned int sample;
byte counter = 0;

AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN, AUDIO_SAMPLE_RATE_HZ, AUDIO_SAMPLE_LENGTH); // sample input from the microphone
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_NEOPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
/**
 * boot the chip
 */
void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(10, OUTPUT);

}

void loop() {
  // this is the last thing we do before taking over the CPU
  audioSampler.sample(fht_input, audioSampleCallback);
}



/** 
 * this function is returned every AUDIO_SAMPLE_LENGTH/SAMPLE_RATE_HZ seconds.
 * it takes in discrete audio sample values taken every 1/SAMPLE_RATE_HZ seconds over AUDIO_SAMPLE_LENGTH 
 * it analyzes the waveform for frequency magnitudes, stores the values in a history, then displays into a pixel output
 */

void audioSampleCallback(void) {
/*
   // process data
   fht_window(); // window the data for better frequency response
   fht_reorder(); // reorder the data before doing the fht
   fht_run(); // process the data in the fht
   //fht_mag_octave(); // take the output of the fht, save into fht_oct_out[]
   // this will result in log(N) bins
   // apparently the input is takes from fft_input?
   
   // alternatevly, can use fht_mag_lin, which returns fht_lin_out[] linearly
   
   fht_mag_log(); // take the output of the fht
   //fft_mag_octave();
   
   for (byte i = 0 ; i < AUDIO_SAMPLE_LENGTH/2 ; i++) {
   Serial.print(fht_log_out[i]);
   //Serial.print(fft_oct_out[i]);
   Serial.print(",");
   }
   Serial.println();
   
   
   //Serial.write(255);
   //Serial.write(fht_log_out, AUDIO_SAMPLE_LENGTH/2);
   
   
   
   Serial.println("sample taken");
   Serial.println(counter);
   // audio sample callback happens here.
   counter++;
   if (counter > 255) {
   counter = 0;
   }
   
   */

  // get the max different in the fft_input
  int minValue = 1024;
  int maxValue = 0;
  int peakChange = 0;
  int normalizedPeak = 0;
  for (unsigned int sampleIndex = 0; sampleIndex < AUDIO_SAMPLE_LENGTH; sampleIndex++) {
    //Serial.print(fht_input[sampleIndex]);
    //Serial.print(",");
    if (fht_input[sampleIndex] < minValue) minValue = fht_input[sampleIndex]; 
    if (fht_input[sampleIndex] > maxValue) maxValue = fht_input[sampleIndex]; 
  }

  peakChange = maxValue - minValue;


  Serial.print(minValue);
  Serial.print(":");
  Serial.print(maxValue);
  Serial.print(" = ");
  Serial.print(peakChange);
  Serial.println();

  normalizedPeak = ceil(NUM_NEOPIXELS*peakChange/1024);


  uint32_t color = strip.Color(96, 0, 0);

  for(byte i=0; i<strip.numPixels(); i++) {
    if (i <= normalizedPeak) {
      strip.setPixelColor(i, color);
    } 
    else {
      strip.setPixelColor(i, strip.Color(0,0,0)); // turn pixel off
    }

  }
  strip.show();
  
  //analogWrite(10, 255*peakChange/1024);
  //analogWrite(10, 255);
  /* */
}


