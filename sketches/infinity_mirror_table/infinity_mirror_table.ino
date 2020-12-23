/**
 * Infinity table lights - reacts to sound by showing volume as light intensity,
 * then propogating the light intensities down a neopixel strip around the table
 * 
 * @author Tony Gaitatzis 2015
 */
#include "SimpleNeopixel.h"
#include "AudioSampler.h"
#include "InfinityLights.h"
#include <avr/power.h> // required to make 5V trinket run at 16mHz


////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE CONSTANTS
// Change these to change how the program records and processes data
////////////////////////////////////////////////////////////////////////////////

/** 
 * System Configuration
 */
const byte POWER_LED_PIN = 1;


/**
 * Audio Sampling Configuration
 */
const unsigned int AUDIO_SAMPLE_RATE_HZ = 22050;             // Sample rate of the audio in hertz.
const unsigned int AUDIO_SAMPLE_LENGTH = 16;
const byte AUDIO_INPUT_PIN = A2;        // Input ADC pin for audio data.


/**
 * InfinityLights Configuration
 */
const byte NEOPIXEL_PIN = 0;
const unsigned int NUMPIXELS = 64;



/**
 * Animation Configuration
 */
const unsigned int animationFrameTimeout_ms = 100;





////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

unsigned long lastAnimationFrameUpdateTime;
int samples[AUDIO_SAMPLE_LENGTH];

AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN, AUDIO_SAMPLE_RATE_HZ, AUDIO_SAMPLE_LENGTH); // sample input from the microphone
InfinityLights infinityLights = InfinityLights(NUMPIXELS, NEOPIXEL_PIN);



void setup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH); 
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastAnimationFrameUpdateTime > animationFrameTimeout_ms) {
  
    audioSampler.sample(samples);
    
    byte pixelValue = audioSampler.getIntensity(samples)/4;
    
    
    analogWrite(POWER_LED_PIN, pixelValue);  // diagnostic led
    
    infinityLights.update(pixelValue);
    infinityLights.display();
    
    lastAnimationFrameUpdateTime = currentTime;
  }
  
}

