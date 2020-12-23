#define ARRAYSIZE(a)  ((sizeof(a) / sizeof(*(a))) / static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
  
#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht

#include <FHT.h>
#include <Adafruit_NeoPixel.h>
#include "gammaCorrection.h"
#include "ColorConverter.h"

/** NeoPixel settings **/
const uint8_t pixelPin = 6;
const uint8_t pixelWidth = 8;
const uint8_t pixelHeight = 8;
const uint8_t numPixels = pixelWidth * pixelHeight;
const uint16_t animationDelay_ms = 0;

const uint8_t hue = 16;
const uint8_t saturation = 80;


/** Microphone settings **/
const uint8_t micPin = A0;
const uint16_t numMicReadings = 255;
const uint16_t numMicSamples = 255;

/** Runtime variables **/


/** Function prototypes **/
uint16_t getMicRange(uint8_t micPin, uint16_t numSamples);
void arrayPush(int** array, uint8_t value);


struct LoopTimer {
  uint16_t startTime_ms;
  uint16_t delayTime_ms;
  uint16_t expirationTime_ms;
  LoopTimer(uint16_t incomingDelayTime_ms) {
    startTime_ms = millis();
    delayTime_ms = incomingDelayTime_ms;
    expirationTime_ms = startTime_ms + delayTime_ms;
  }
  bool isExpired() {
    uint16_t currentTime_ms = millis();
    if (currentTime_ms > expirationTime_ms) {
      expirationTime_ms = currentTime_ms + delayTime_ms;
      return true;
    }
  }
};


/** Program **/

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numPixels, pixelPin,  NEO_GRB + NEO_KHZ800);
LoopTimer loopTimer = LoopTimer(animationDelay_ms);

void setup() {
  Serial.begin(9600);
  pixels.begin();

  pinMode(micPin, INPUT);
  
}

void loop() {
  while (1) { // this loop reduces jitter
    cli(); // turn off interrupts to speed things up
    
    // animation delay
    if (!loopTimer.isExpired()) return;
  
    // read mic 
    uint16_t micSample = getMicRange(micPin, numMicReadings);
    
    // push data down the array
    for (int i = 10; i > 0; i--) {
//    for (int i = ARRAYSIZE[fht_input] - 1; i > 0; i--) {
      fht_input[i] = fht_input[i-1];
    }
    fht_input[0] = micSample;

    for (int i=0; i < 10; i++) {
      Serial.print(i);
      Serial.print(":");
      Serial.print(fht_input[i]);
      Serial.print("\t");
    }
    Serial.println();
    
    // break apart into fht
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_log(); // take the output of the fht
    sei(); // turn interrupts off again

    
    // plot onto neopixels
    for (int row = 0; row < pixelWidth; row++) {
      int pixelStart = row * pixelHeight;
      for (int i=0; i < fht_log_out[row]; i++) {
        uint8_t pixel = pixelStart + i;
        uint8_t value = 65;
        HsvColor hsvColor;
        hsvColor.h = hue;
        hsvColor.s = saturation;
        hsvColor.v = value;
        RgbColor rgbColor = ColorConverter::HsvToRgb(hsvColor);
        pixels.setPixelColor(pixel, pixels.Color(gammaCorrect(rgbColor.r),gammaCorrect(rgbColor.g),gammaCorrect(rgbColor.b)));
      }
    }
    
  }
}


void arrayPush(int** array, uint8_t value) {
}

int gammaCorrect(int input) {
  return pgm_read_byte(&gamma8[input]);
}


uint16_t getMicRange(uint8_t micPin, uint16_t numReadings) {
  uint16_t micMax = 0;
  uint16_t micMin = 1023;
  for (int i=0; i < numReadings; i++) {
    uint16_t micVal = analogRead(micPin);
    micMax = max(micMax, micVal);
    micMin = min(micMin, micVal);
  }

  uint16_t micRange = micMax - micMin;
  return micRange;
}

