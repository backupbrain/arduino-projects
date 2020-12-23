#include <Adafruit_NeoPixel.h>


#define ARM_MATH_CM4
#include <arm_math.h>
#include "BeatAnalyzer.h"

int SAMPLE_RATE_HZ = 9000;             // Sample rate of the audio in hertz.
float SPECTRUM_MIN_DB = 30.0;          // Audio intensity (in decibels) that maps to low LED brightness.
float SPECTRUM_MAX_DB = 60.0;          // Audio intensity (in decibels) that maps to high LED brightness.


const int FFT_RESOLUTION = 256;        // Size of the FFT.  Realistically can only be at most 256
                                       // without running out of memory for buffers and other state. 
const int AUDIO_INPUT_PIN = 14;        // Input ADC pin for audio data.
const int ANALOG_READ_RESOLUTION = 10; // Bits of resolution for the ADC.
const int ANALOG_READ_AVERAGING = 16;  // Number of samples to average with each ADC reading.
const int POWER_LED_PIN = 13;          // Output pin for power LED (pin 13 to use Teensy 3.0's onboard LED).
const int NEO_PIXEL_PIN = 3;           // Output pin for neo pixels.
const int NEO_PIXEL_COUNT = 64;        // Number of neo pixels.  You should be able to increase this 
                                       // any other changes to the program.
const int MAX_CHARS = 65;              // Max size of the input command buffer

const int LED_PIN = 10;

////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

IntervalTimer samplingTimer;
float samples[FFT_RESOLUTION*2];
int sampleCounter = 0;
float frequencyMagnitudes[FFT_RESOLUTION];

boolean beatDetected = false;
BeatAnalyzer beatAnalyzer(SAMPLE_RATE_HZ);

float startTime;
float currentTime;
float timeout = 5000; // ms


static const unsigned int numNeoPixels = 64;
static const unsigned int neopixel_pin = 3;
Adafruit_NeoPixel pixelStrip = Adafruit_NeoPixel(numNeoPixels, neopixel_pin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  pinMode(AUDIO_INPUT_PIN, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);
  
  
  pixelStrip.begin();
  pixelStrip.show(); 
  
  // Begin sampling audio
  samplingBegin();
  
  startTime = millis();
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixelStrip.numPixels(); i++) {
      pixelStrip.setPixelColor(i, c);
      pixelStrip.show();
      delay(wait);
  }
}

void loop() {
  beatDetected = false;
  
  currentTime = millis();
  
  // Calculate FFT if a full sample is available.
  if (samplingIsDone()) {   
   
    // Run FFT on sample data.
    arm_cfft_radix4_instance_f32 fft_inst;
    arm_cfft_radix4_init_f32(&fft_inst, FFT_RESOLUTION, 0, 1);
    arm_cfft_radix4_f32(&fft_inst, samples);
    // Calculate magnitude of complex numbers output by the FFT.
    arm_cmplx_mag_f32(samples, frequencyMagnitudes, FFT_RESOLUTION);
    
    // reset the display
    for (int p=0; p<64; p++) {
     pixelStrip.setPixelColor(p, pixelStrip.Color(0, 0, 0));
    }
    
    float* historyBuffer = beatAnalyzer.getHistoryBuffer();
    float* averageIntensities = beatAnalyzer.getAverageIntensities();
    
    
    
      
      
      
    for (int j=0; j<8; j++) {
      pixelStrip.setPixelColor(j, pixelStrip.Color(0, 10*averageIntensities[j], 0));
      
      Serial.print(averageIntensities[j]);
      Serial.print(",");
    }
    
    Serial.println("");
    
    
    float intensity;
    for (int i=8; i<64; i++) {
      Serial.print(historyBuffer[i]);
      
      intensity = historyBuffer[i];
      // Scale the intensity and clamp between 0 and 1.0.
      intensity -= SPECTRUM_MIN_DB;
      intensity = intensity < 0.0 ? 0.0 : intensity;
      intensity /= (SPECTRUM_MAX_DB-SPECTRUM_MIN_DB);
      intensity = intensity > 1.0 ? 1.0 : intensity;
      pixelStrip.setPixelColor(i, pixelStrip.Color(0, 10*intensity, 0));
 // colorWipe(pixelStrip.Color(255, 0, 0), 50); // Red
      
     
      //intensity = 
      //pixelStrip[i].setPixelColor(intensity);
      Serial.print(",");
    }
    
    
    float beatProbability = beatAnalyzer.getBeatProbability();
    for (int x=0; x<64; x+=8) {
      if (beatProbability > (x/128)) { 
          pixelStrip.setPixelColor(x, pixelStrip.Color(10, 10, 10));
      }
        
    }
    
      pixelStrip.show();
    Serial.println("");
    
    
    int subBandSize = 32;
    float subBands[subBandSize];
    beatDetected = beatAnalyzer.detectBeat(frequencyMagnitudes, FFT_RESOLUTION);
    if (beatDetected) {
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13,LOW);
    }
    
    
    if ((currentTime - startTime) < timeout) {
      Serial.print("Beat: ");
      Serial.println(beatDetected);
     /*
      for (int i=0; i<FFT_RESOLUTION; i++) {
        Serial.print(frequencyMagnitudes[i]);
        Serial.print(",");
      }
      Serial.println("-"); 
      */
    }
    
    // Restart audio sampling.
    samplingBegin();
  }
}



void samplingBegin() {
  // Reset sample buffer position and start callback at necessary rate.
  sampleCounter = 0;
  samplingTimer.begin(samplingCallback, 1000000/SAMPLE_RATE_HZ);
}
void samplingCallback() {
  // Tony's notes:
  // The sampling rate is 1000000/SAMPLE_RATE_HZ (default 9000)
  // so the sampling interval is about 0.000111 seconds.
  // Each 0.000111 seconds, we read a signal and dump it into samples,
  // until samples fills up to FFT_SIZE (*2 includes imaginary numbers). (default 256)
  // That means there a new sample window is created every 0.028 seconds
  // the sample window creation frequency is 35 hz
  
  // Read from the ADC and store the sample data
  samples[sampleCounter] = (float32_t)analogRead(AUDIO_INPUT_PIN);
  
  
  // incoming default value is 1v (~340)
  // want to convert to intensity, which is |1v-value|
  int intensity = (340-samples[sampleCounter])/4;
  if (intensity < 0) {
    intensity = - intensity;
  }
  analogWrite(LED_PIN, intensity);
  
  //Serial.print(samples[sampleCounter]);
  // Complex FFT functions require a coefficient for the imaginary part of the input.
  // Since we only have real data, set this coefficient to zero.
  samples[sampleCounter+1] = 0.0;
  // Update sample buffer position and stop after the buffer is filled
  sampleCounter += 2;
  if (sampleCounter >= FFT_RESOLUTION*2) {
    samplingTimer.end();
  }
}


boolean samplingIsDone() {
  return sampleCounter >= FFT_RESOLUTION*2;
}
