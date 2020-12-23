
#include "AudioSampler.h"
#include "FixedFFT.h"
#include "RgbLed.h"



////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE CONSTANTS
// Change these to change how the program records and processes data
////////////////////////////////////////////////////////////////////////////////


/** 
 * System Configuration
 */
const byte POWER_LED_PIN = 1;
const boolean POWER_LED_ON = false;


/**
 * Audio Sampling Configuration
 */
const unsigned int AUDIO_SAMPLE_RATE_HZ = 44100;             // Sample rate of the audio in hertz.
const unsigned int AUDIO_SAMPLE_LENGTH = 128; // was 1024
const byte AUDIO_INPUT_PIN = 0;        // Input ADC pin for audio data.



/**
 * Audio Processing Configuration
 */
const unsigned int SPECTRUM_MIN_DB = 30;
const unsigned int SPECTRUM_MAX_DB = 90;
const unsigned int MIN_SOUND_VOLUME = 0;
const unsigned int FFT_RESOLUTION = AUDIO_SAMPLE_LENGTH;              // Size of the FFT.  Realistically can only be at most 256 


/**
 * RgbLED Configuration
 */
const char PORT = 'B';


////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

const byte hue_increment = 1;
char samples[AUDIO_SAMPLE_LENGTH];
char imaginary[AUDIO_SAMPLE_LENGTH];

byte fht_values[FFT_RESOLUTION]; 

unsigned int lowFrequencyMax = 0;
unsigned int midFrequencyMax = 0;

HsvColor hsvColor;
AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN, AUDIO_SAMPLE_RATE_HZ, AUDIO_SAMPLE_LENGTH); // sample input from the microphone
RgbLED rgbLED = RgbLED(PORT, LED_TYPE_COMMON_ANODE);


void setup() {
  if (POWER_LED_ON) {
    pinMode(POWER_LED_PIN, OUTPUT);
  }
}

void loop() {  
    audioSampler.sample(samples);
    byte normalizedVolume = audioSampler.getIntensity(samples)/16;
    
    analyzeFrequencies(samples);
  
    // get rid of the "volume metric on the fht
    for (int x=0; x < FFT_RESOLUTION; x++) {
      //fht_values[x] = fht_oct_out[x+2];
      fht_values[x] = samples[x+2];
      if (fht_values[x] < MIN_SOUND_VOLUME) {
        fht_values[x] = 0;
      } else {
        fht_values[x] = samples[x] - MIN_SOUND_VOLUME;
      }
    }

    // average the low frequencies
    float lowFrequencyVolume = 0;
    unsigned char numLowBands = 32;
    for (int x=0; x< numLowBands; x++) {
      lowFrequencyVolume += fht_values[x];
      if (fht_values[x] > lowFrequencyMax) {
        lowFrequencyMax = fht_values[x];
      }
    }
    lowFrequencyVolume = lowFrequencyVolume/numLowBands;
    
    
    float midFrequencyVolume = 0;
    unsigned char numMidBands = 64;
    for (int x=numLowBands; x<numMidBands; x++) {
      midFrequencyVolume += fht_values[x];
      if (fht_values[x] > lowFrequencyMax) {
        midFrequencyMax = fht_values[x];
      }
    }
    midFrequencyVolume = midFrequencyVolume/numLowBands;
    
    unsigned char valueValue = map(lowFrequencyVolume, 0, lowFrequencyMax, 0, 255);
    unsigned char hueValue = map(midFrequencyVolume, 0, midFrequencyMax, 0, 255);
    
    // map low channels to value
    // map mid channels to hue?
    
    hsvColor.v = valueValue;
    hsvColor.h = hueValue;
    
  
    RgbColor rgbColor = ColorConverter::HsvToRgb(hsvColor);
    
    
    rgbLED.setColor(rgbColor);
    rgbLED.output();
}


byte* analyzeFrequencies(char* samples) {
  // run the fft
  FixedFFT::fftr(samples,7,0);

}

