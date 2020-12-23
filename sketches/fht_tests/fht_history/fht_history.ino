#include <Adafruit_NeoPixel.h>
#include "AudioSampler.h"
#include "FrequencyHistory.h"



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
const unsigned int AUDIO_SAMPLE_LENGTH = 128;
const byte AUDIO_INPUT_PIN = A0;        // Input ADC pin for audio data.

float historyLength_seconds = 0.25;
//const unsigned int HISTORY_LENGTH = (int)floor(historyLength_seconds)*(AUDIO_SAMPLE_RATE_HZ/AUDIO_SAMPLE_LENGTH);


/**
 * Audio Processing Configuration
 */
const unsigned int SPECTRUM_MIN_DB = 30;
const unsigned int SPECTRUM_MAX_DB = 90;
const unsigned int FFT_RESOLUTION = 256;              // Size of the FFT.  Realistically can only be at most 256 


/** 
 * FHT library Configuration for arduino
 */
#define LOG_OUT 1 // use the log output function
#define OCTAVE 1 // use the fft_mag_octave output function
#define FHT_N 256 // set to 256 point fht, will initialize: int fht_input[FHT_N];
#include <FHT.h>


/**
 * Frequency History Configuration
 */
const unsigned int FREQUENCY_HISTORY_RESOLUTION = 8;
const unsigned int HISTORY_LENGTH = 8;


/**
 * Display Configuration
 */
const byte NUM_NEOPIXELS = 64;
const byte NEOPIXEL_PIN = 9;


////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

unsigned int sample;
int samples[AUDIO_SAMPLE_LENGTH];
byte counter = 0;

AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN, AUDIO_SAMPLE_RATE_HZ, AUDIO_SAMPLE_LENGTH); // sample input from the microphone
FrequencyHistory frequencyHistory = FrequencyHistory(HISTORY_LENGTH, FREQUENCY_HISTORY_RESOLUTION, FHT_N); // sample input from the microphone
//FHTSpectrumAnalyzer spectrumAnalyzer = FHTSpectrumAnalyzer(FFT_RESOLUTION, AUDIO_SAMPLE_RATE_HZ); // FFT functionality

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
  audioSampler.sample(samples);
  
  analyzeFrequencies(samples);
  
  //frequencyHistory.loadFromFHT(fht_log_out);
  //frequencyHistory.loadFromFHTOctave(fht_oct_out);
  
  serial_out_log();
 // serial_out_octave();
  
  //displayFFTHistory(frequencyHistory.getData());
  displayFFT();
}

void serial_out_octave() {
  for (int i=0; i<LOG_N; i++) {
    if (i>0) {
      Serial.print(",");
    }
    Serial.print(fht_oct_out[i]);
  }
  Serial.println();
}
void serial_out_log_partial() {
  int initialValue = 2;
  for (int i=initialValue; i<FREQUENCY_HISTORY_RESOLUTION+initialValue; i++) {
    if (i>initialValue) {
      Serial.print(",");
    }
    Serial.print(fht_log_out[i]);
  }
  Serial.println();
}

void serial_out_log() {
  for (int i=0; i<FHT_N/2; i++) {
    if (i>0) {
      Serial.print(",");
    }
    Serial.print(fht_log_out[i]);
  }
  Serial.println();
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


void displayFFTHistory(byte* history) {
  
   unsigned int numPixels = strip.numPixels();
   unsigned int startIndex = 0;
   unsigned int endIndex = 0;
   uint32_t color;
   unsigned int colorValue = 0;
   byte value;
   
   byte pixelPosition = 0;
   /*
   for (unsigned int pos = 0; pos < 64; pos++) {
       colorValue = 64;
       color = strip.Color(colorValue, 0, 0);
       strip.setPixelColor(pos, color);
     
   }
  /* */
   for (unsigned int historyPosition = 0; historyPosition < HISTORY_LENGTH; historyPosition++) {
     //pixelPosition = historyPosition * FREQUENCY_HISTORY_RESOLUTION;
     for (unsigned int frequencyIndex = 0; frequencyIndex < FREQUENCY_HISTORY_RESOLUTION; frequencyIndex++) {
       pixelPosition = frequencyIndex + historyPosition * FREQUENCY_HISTORY_RESOLUTION;
       
       
       value = frequencyHistory.getValue(frequencyIndex, historyPosition);
       colorValue = (uint32_t) value;
       color = strip.Color(colorValue, 0, 0);
       strip.setPixelColor(pixelPosition, color);
       
       pixelPosition++;
     }
   }
   /* */
   strip.show();
}

void displayFFT() {
  
   unsigned int numPixels = strip.numPixels();
   unsigned int startIndex = 0;
   unsigned int endIndex = 0;
   uint32_t color;
   unsigned int colorValue = 0;
   for (unsigned int pixel = 0; pixel < numPixels; pixel++) {
     
     colorValue = fht_log_out[pixel+2];
     color = strip.Color(colorValue, 0, 0);
     strip.setPixelColor(pixel, strip.Color(colorValue, 0, 0));
     /* */
     /*
     startIndex = floor((float)pixel/strip.numPixels())*FHT_N/2;
     //if (startIndex < 2) startIndex = 2; 
     
     endIndex = floor(float(pixel+1)/strip.numPixels())*FHT_N/2;
     //if (endIndex < 2) endIndex = 2; 
     
     colorValue = 0;
     for (unsigned int frequencyIndex = startIndex; frequencyIndex < endIndex; frequencyIndex++) {
       colorValue += fht_log_out[frequencyIndex];
     }
     colorValue = (float)colorValue/(endIndex-startIndex);
     Serial.print(colorValue);
     Serial.print(",");
     color = strip.Color(colorValue, 0, 0);
     strip.setPixelColor(pixel, strip.Color(colorValue, 0, 0));
     /* */
   }
   //Serial.println();
   strip.show();
   
   /*
   for (byte i = 2 ; i < FHT_N/2 ; i++) {
     //Serial.print(fht_oct_out[i]);
     Serial.print(fht_log_out[i]);
     Serial.print(",");
   }
   Serial.println();
   /* */
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
   
   //fht_mag_lin();
   fht_mag_log(); // take the output of the fht
   //fht_mag_octave();
   */

   /*
   unsigned int numPixels = strip.numPixels();
   unsigned int startIndex = 0;
   unsigned int endIndex = 0;
   uint32_t color;
   unsigned int colorValue = 0;
   for (unsigned int pixel = 0; pixel < numPixels; pixel++) {
     startIndex = floor(pixel/strip.numPixels())*FHT_N/2;
     if (startIndex < 1) startIndex = 1; 
     endIndex = floor(pixel+1/strip.numPixels())*FHT_N/2;
     colorValue = 0;
     for (unsigned int frequencyIndex = startIndex; frequencyIndex < endIndex; frequencyIndex++) {
       colorValue += fht_log_out[frequencyIndex];
       //colorValue += fht_oct_out[frequencyIndex];
     }
     colorValue = colorValue/(endIndex-startIndex);
     //Serial.print(colorValue);
     //Serial.print(",");
     color = strip.Color(colorValue, 0, 0);
     strip.setPixelColor(pixel, strip.Color(colorValue, 0, 0));
   }
   strip.show();
   //Serial.println();
   
   
   for (byte i = 2 ; i < FHT_N/2 ; i++) {
     //Serial.print(fht_oct_out[i]);
     Serial.print(fht_log_out[i]);
     Serial.print(",");
   }
   Serial.println();
   
   
   //Serial.write(255);
   //Serial.write(fht_log_out, AUDIO_SAMPLE_LENGTH/2);
   
   /* */

/*
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
  /* */
  
  //analogWrite(10, 255*peakChange/1024);
  //analogWrite(10, 255);
  /* */
}


