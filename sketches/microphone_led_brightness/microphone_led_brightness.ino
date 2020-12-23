#include <Adafruit_NeoPixel.h>
#include "AudioSampler.h"
#include "SpectrumAnalyzer.h"
#include <MemoryFree.h>

/****************************************
Example Sound Level Sketch for the 
Adafruit Microphone Amplifier
****************************************/

#define LED_PIN 13
#define MIC_PIN A0



int SAMPLE_RATE_HZ = 44100;             // Sample rate of the audio in hertz.

const int AUDIO_INPUT_PIN = A14;        // Input ADC pin for audio data.

const int FFT_RESOLUTION = 256;              // Size of the FFT.  Realistically can only be at most 256 

const int AUDIO_SAMPLE_LENGTH = 256;
const int NEOPIXEL_WIDTH = 8;
const int NEOPIXEL_HEIGHT = 1;

const int SPECTRUM_MIN_DB = 30;
const int SPECTRUM_MAX_DB = 90;

const int NEOPIXEL_PIN = 0;

const int POWER_LED_PIN = 13;

unsigned int sample;


const byte AUDIO_OUT_PIN = A14;       // DAC pin
const unsigned int AUDIO_OUT_MIN_VALUE = 0;    // min output of DAC
const unsigned int AUDIO_OUT_MAX_VALUE = 256; // max output of DAC
unsigned long startTime = 0;
unsigned long currentTime = 0;





const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;


AudioSampler audioSampler = AudioSampler(AUDIO_INPUT_PIN); // sample input from the microphone

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_WIDTH*NEOPIXEL_HEIGHT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
   Serial.begin(9600);
   pinMode(LED_PIN, OUTPUT);
}


void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level

   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;

   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(MIC_PIN);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 3.3) / 1024;  // convert to volts
   
   // convert peakToPeak (0-1024) to 0-255 for light
   int brightness = peakToPeak / 4;
   analogWrite(LED_PIN, brightness); 
  
   

/*
      strip.setPixelColor(i, c);
      strip.show();
      */

   Serial.println(volts);
}


/*
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
*/

