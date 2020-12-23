#include "AudioSampler.h"
#include "FHTHistory.h"
#include "WaterfallPlotter.h"
#include "RgbLed.h"

const byte ledPin = 13;

const byte neopixelWidth = 8;
const byte neopixelHeight = 8;
const byte neopixelPin = 6;


/** 
 * FHT library Configuration for arduino
 */
#define LOG_OUT 1 // use the log output function
//#define OCTAVE 1 // use the fft_mag_octave output function
//#define OCT_NORM 0
#define FHT_N 128 // set to 256 point fht, will initialize: int fht_input[FHT_N];
#include <FHT.h>



/**
 * Audio Sampling Configuration
 */
const unsigned int AUDIO_SAMPLE_RATE_HZ = 44100;             // Sample rate of the audio in hertz.
const unsigned int AUDIO_SAMPLE_LENGTH = 128; // was 1024
const byte AUDIO_INPUT_PIN = A0;        // Input ADC pin for audio data.



/**
 * Audio Processing Configuration
 */
const unsigned int SPECTRUM_MIN_DB = 30;
const unsigned int SPECTRUM_MAX_DB = 90;
const unsigned int MIN_SOUND_VOLUME = 0;
const unsigned int FFT_RESOLUTION = FHT_N/2;              // Size of the FFT.  Realistically can only be at most 256 


/**
 * RgbLED Configuration
 */
const char PORT = 'B';


HsvColor hsvColor;

FHTHistory fhtHistory = FHTHistory(neopixelWidth, neopixelHeight, FFT_RESOLUTION);
AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN, AUDIO_SAMPLE_RATE_HZ, AUDIO_SAMPLE_LENGTH); // sample input from the microphone
RgbLED rgbLED = RgbLED(PORT, LED_TYPE_COMMON_ANODE);



int samples[AUDIO_SAMPLE_LENGTH];

byte fht_values[FFT_RESOLUTION]; 

void setup()
{
    pinMode(ledPin, OUTPUT);
  pixelStrip.begin();
}

void loop()
{
  /*
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(900);
    /* */



  
    audioSampler.sample(samples);
    byte normalizedVolume = audioSampler.getIntensity(samples)/16;
    
    
    analyzeFrequencies(samples);
  

  
    // get rid of the "volume metric on the fht
    for (int x=0; x < FFT_RESOLUTION; x++) {
      //fht_values[x] = fht_oct_out[x+2];
      fht_values[x] = fht_log_out[x+2];
      /*
      if (fht_values[x] < MIN_SOUND_VOLUME) {
        fht_values[x] = 0;
      } else {
        fht_values[x] = fht_values[x] - MIN_SOUND_VOLUME;
      }
      /* */
    }
    
  
    fhtHistory.loadFromFHT(fht_values);

    waterfallPlotter.update(fhtHistory.getData());
    waterfallPlotter.display();
  
    
    /*
    for (int x=0; x < FHT_N/2; x++) {
      Serial.print(fht_log_out[x]);
      Serial.print(",");
    }
    Serial.println();
    /* */
  
    //fhtHistory.loadFromFHT(fht_values);
  /*
  // stream the fft - each pixel is a channel
  for (int pixel = 0; pixel < 64; pixel++) {
    byte pixelValue = fht_log_out[pixel];
  	pixelStrip.setPixelColor(pixel, pixelValue, pixelValue, pixelValue);
  }
  pixelStrip.show();
  /* */
  
  /*
  // stream the fft
  byte pixelValue = 0;
  byte pixel = 0;
  for (int x = 0; x < neopixelWidth; x++) {
    
    // push pixel values across panel
    for (int y = neopixelHeight-1; y > 0; y--) {
      // get the current pixel in the grid
      pixel = x + y * neopixelWidth;
      pixelValue = pixelStrip.getPixelColor(pixel - 1);
  	  pixelStrip.setPixelColor(pixel, pixelValue, pixelValue, pixelValue);
    }
    
    pixelValue = fht_values[x];
    pixelStrip.setPixelColor(x, pixelValue, pixelValue, pixelValue);
  }
  pixelStrip.show();
  /* */

  /*
  // stream the volume
  for (pixel = 63; pixel > 0; pixel--) {
    byte pixelValue = pixelStrip.getPixelColor(pixel-1);
    pixelStrip.setPixelColor(pixel, pixelValue, pixelValue, pixelValue);
  }
  pixelStrip.setPixelColor(0, normalizedVolume);
  pixelStrip.show();
  /* */
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

