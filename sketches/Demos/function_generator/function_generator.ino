
#include <math.h> // for sin

//#define PI 3.14159265;
const byte AUDIO_OUT_PIN = A14;       // DAC pin
const unsigned int AUDIO_OUT_MIN_VALUE = 0;    // min output of DAC
const unsigned int AUDIO_OUT_MAX_VALUE = 256; // max output of DAC
unsigned long startTime = 0;
unsigned long currentTime = 0;

unsigned int frequency_hz = 440;          // desired sine frequency in Hz
unsigned int outputValue = 0;             // output value of DAC



void setup() {
  
  analogWriteResolution(12);
  startTime = millis();
}


void loop() {
  currentTime = millis();
  
  // want to generate a sine wave of 440 hz
  long timeDifference = currentTime - startTime;
  
  // time on the computer is measured in milliseconds
  // one second is 1000 ms
  // Each period of 440 hz happens over 2.27 ms (0.44 periods/ms)
  
  // C thinks of sine(x) in degrees
  
  outputValue = (AUDIO_OUT_MAX_VALUE/2)*sin(timeDifference)+(AUDIO_OUT_MAX_VALUE/2);
  
  
  analogWrite(AUDIO_OUT_PIN, (int)outputValue);
}
