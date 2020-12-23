#include <MemoryFree.h>

#include "AudioSampler.h"
#include "FHTChannelHistory.h"
#include "BeatAnalyzer.h"
#include "LightPipe.h"

////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE CONSTANTS
// Change these to change how the program records and processes data
////////////////////////////////////////////////////////////////////////////////

/** 
 * System Configuration
 */
const uint8_t POWER_LED_PIN = 13;

/**
 * Audio Sampling Configuration
 */
const uint16_t AUDIO_SAMPLE_RATE_HZ = 44100;             // Sample rate of the audio in hertz.
const uint16_t AUDIO_SAMPLE_LENGTH = 128;
const uint8_t AUDIO_INPUT_PIN = A0;        // Input ADC pin for audio data.



/**
 * Audio Processing Configuration
 */
const uint8_t SPECTRUM_MIN_DB = 30;
const uint8_t SPECTRUM_MAX_DB = 90;


/** 
 * FHT library Configuration for arduino
 */
//#define LOG_OUT 1 // use the log output function
#define OCTAVE 1 // use the fft_mag_octave output function
#define OCT_NORM 0
#define FHT_N 128 // set to 256 point16_t fht, will initialize: int16_t fht_input[FHT_N];
#include <FHT.h>

/**
 * FHTSpectrumAnalyzer Configuration
 */
// 128 of 44100 samples per second = 2.9 milliseconds.  
// roughly 340 samples per second.  
// so if we want 1 second of history, then we want 340 samples
// HISTORY_BUFFER_LENGTH = sample_length_seconds * 340
// WARNING: an arduino may only have 1 or 2 kb or so of RAM to work with
float historyLength_seconds = 1.0;
const uint16_t HISTORY_BUFFER_LENGTH = 340; //(int16_t)floor(historyLength_seconds*(AUDIO_SAMPLE_RATE_HZ/AUDIO_SAMPLE_LENGTH));

/**
 * BeatAnalyzer Configuration
 */
const uint8_t beatChannel = 2;

/**
 * Display Configuration
 */
const uint8_t LIGHTPIPE_PIN = 9;


////////////////////////////////////////////////////////////////////////////////
// int16_tERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

uint16_t sample;
int16_t samples[AUDIO_SAMPLE_LENGTH];
uint8_t counter = 0;

AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN, AUDIO_SAMPLE_RATE_HZ, AUDIO_SAMPLE_LENGTH); // sample input from the microphone
FHTChannelHistory channelHistory = FHTChannelHistory(HISTORY_BUFFER_LENGTH);
BeatAnalyzer beatAnalyzer = BeatAnalyzer();
LightPipe lightPipe = LightPipe(LIGHTPIPE_PIN);

/**
 * boot the chip
 */
void setup() {
  Serial.begin(115200);
  
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH);
}

void loop() {
  audioSampler.sample(samples);
  
  analyzeFrequencies(samples);
  
  //Serial.println(fht_oct_out[beatChannel]);
  channelHistory.loadFromFHT(fht_oct_out[beatChannel]);
  
  beatAnalyzer.getAverageVolumeForFrequency(channelHistory.getData(), HISTORY_BUFFER_LENGTH);
  uint8_t beatProbability = beatAnalyzer.calculateBeatProbability(fht_oct_out[beatChannel]);
  
  lightPipe.react(beatProbability);
  lightPipe.display();
  
  
  //printChannelData();
  //printData(beatProbability);
  
  //Serial.print("freeMemory()=");
  //Serial.println(freeMemory());
  
  //displayFFT();
}

void printChannelData() {
  for (int16_t frequency=0; frequency<8; frequency++) {
    Serial.print(fht_oct_out[frequency]);
    Serial.print(",");
  }
  Serial.println();
}

void printData(uint8_t beatProbability) {
  uint8_t ledBrightness = lightPipe.getBrightness();
  
  Serial.print("{");
  
  /*
  Serial.print("frequencies: ");
  Serial.print("{");
  for (uint16_t frequency = 0; frequency < FHT_N; frequency++) {
    if (frequency > 0) {
      Serial.print(",");
    }
    Serial.print(fht_oct_out[frequency]);
  }
  Serial.print("}");
  Serial.print(", ");
  /* */
  
  Serial.print("frequency_history: ");
  Serial.print("{");
  for (uint16_t historyPosition = 0; historyPosition < HISTORY_BUFFER_LENGTH; historyPosition++) {
    if (historyPosition > 0) {
      Serial.print(",");
    }
    Serial.print(channelHistory.getValue(historyPosition));
  }
  Serial.print("}");
  
  Serial.print(", ");
  
  Serial.print("beat_probability: ");
  Serial.print(beatProbability);
  Serial.print(", ");
  Serial.print("led_brightness: ");
  Serial.print(ledBrightness);

  
  Serial.print("}");
  Serial.println();
}

uint8_t* analyzeFrequencies(int16_t* samples) {
  for (uint16_t sample = 0; sample < AUDIO_SAMPLE_LENGTH; sample++) {
    if (sample < FHT_N) { // fht_input is FHT_N uint8_ts long
      fht_input[sample] = samples[sample];
    }
  }
  // process data
  fht_window(); // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  //fht_mag_octave(); // take the output of the fht, save int16_to fht_oct_out[]
  // this will result in log(N) bins
  // apparently the input is takes from fft_input?
   
  // alternatevly, can use fht_mag_lin, which returns fht_lin_out[] linearly
   
  //fht_mag_lin();
  //fht_mag_log(); // take the output of the fht
  fht_mag_octave();
   
  // fht creates and populates fht_log_out
  //return fht_log_out;
  return fht_oct_out;
}

