/**
 * Sound Reactive Table - lights up neopixels in response to sound
 * Created by Tony Gaitatzis, July 5th, 2015
 * Released into the public domain
 * 
 */
#include <avr/power.h>
#include "AudioSampler.h"
#include "Simple_Neopixel.h"


////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE CONSTANTS
// Change these to change how the program records and processes data
////////////////////////////////////////////////////////////////////////////////

/**
 * Light Configuration
 */
const unsigned int NUM_NEOPIXELS = 64;
const byte NEOPIXEL_PIN = 0;

/**
 * Audio Sampling Configuration
 */
const unsigned int AUDIO_SAMPLE_RATE_HZ = 44100;             // Sample rate of the audio in hertz.
const unsigned int AUDIO_SAMPLE_LENGTH = 128; // was 1024
const byte AUDIO_INPUT_PIN = A2;


/**
 * Animation Configuration
 */
const unsigned int animationFrameTimeout_ms = 200;

////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

unsigned long lastAnimationFrameUpdateTime;

unsigned int sample;
int samples[AUDIO_SAMPLE_LENGTH];
byte pixels[NUM_NEOPIXELS];

AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN, AUDIO_SAMPLE_RATE_HZ, AUDIO_SAMPLE_LENGTH); // sample input from the microphone
Simple_Neopixel strip = Simple_Neopixel(NUM_NEOPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

byte i;

void setup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(1, OUTPUT);
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastAnimationFrameUpdateTime > animationFrameTimeout_ms) {
    audioSampler.sample(samples);
    
    byte pixelValue = audioSampler.getIntensity(samples)/4;
    
    analogWrite(1,pixelValue);
    
    pushPixelValue(pixelValue);
    
    
    lastAnimationFrameUpdateTime = currentTime;
  }

}



void pushPixelValue(byte newValue) {
  int halfPoint = floor(strip.numPixels()/2);
  for (int i=halfPoint-1; i>0; i--) {
    pixels[i] = pixels[i-1];
  }
  pixels[0] = newValue;
  for (int i=halfPoint; i<strip.numPixels(); i++) {
    pixels[i] = pixels[i+1];
  }
  pixels[strip.numPixels()-1] = newValue;
  
  cli();
  for (int i=0; i<strip.numPixels(); i++) {
    strip.sendPixel(pixels[i], pixels[i], pixels[i]);
  }
  sei();
  strip.show();
  
  /* */
}
