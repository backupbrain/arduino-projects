// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// REPROGRAMMED for ATTiny85

/* NEOPixels */
// Which pin on the Arduino is connected to the NeoPixels?
//#define PIN            3
#define PIN            0

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      64

//int pixelValues[NUMPIXELS];


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

/* microphone */
const int sampleWindow = 50; // ms
unsigned int sample;
//const int micPin = A0;
const int micPin = A1;

boolean talk = true;




void setup() {
  strip.begin(); // This initializes the NeoPixel library.
  strip.show(); // Initialize all pixels to 'off'
  
  /*
  for (int i = 0; i < (sizeof(pixelValues)/sizeof(int)) - 1; i++) {
    pixelValues[i] = 0;
  }
  /* */
  
  pinMode(1, OUTPUT);
}

void loop() {
  
  
    unsigned long startMillis = millis(); // start of sample window
    unsigned int peakToPeak = 0; // peak-to-peak level
    
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;
    
    int counter = 0;
    while (millis() - startMillis < sampleWindow) {
      sample = analogRead(micPin);
      
      if (talk) { 
//        Serial.println(sample);
      }
      
      if (sample < 1024) {
        if (sample > signalMax) {
          signalMax = sample; // save just the max levels
        } else if (sample < signalMin) {
          signalMin = sample; // save just the min levels
        }
      } 
    }
    
    
    
    if (counter < 2048) {
      counter++;
    } else {
      talk = false;
    }
    
    peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
    double vols = (peakToPeak*3.3)/1024;
    
    int intensity = (int)(peakToPeak/4);
    
    
    analogWrite(1, intensity);
    
    
    pushPixelValue(intensity);
    /*
    colorWipe(strip.Color(intensity, 0, 0), 0); // Red
    
    rainbowCycle(20);
    
   
    // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
    for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      strip.setPixelColor(i, strip.Color(0,150,0)); // Moderately bright green color.
      strip.show(); // This sends the updated pixel color to the hardware.
      delay(delayval); // Delay for a period of time (in milliseconds).
    }
    */
    
}

void pushPixelValue(int newValue) {
  
  int halfPoint = floor(strip.numPixels()/2);
  
  for (int i=halfPoint; i>1; i--) {
    strip.setPixelColor(i, strip.getPixelColor(i-1));
  }
  strip.setPixelColor(0, newValue, newValue, newValue);
  
  for (int i=0; i<strip.numPixels()-1; i++) {
    strip.setPixelColor(i, strip.getPixelColor(i+1));
  }
  strip.setPixelColor(strip.numPixels(), newValue, newValue, newValue);
  strip.show();
  
  /*
  uint16_t i;
  int totalLength = (int)(sizeof(pixelValues)/sizeof(int)) - 1;
  int halfPoint = (int) (totalLength/2);
  for (i = halfPoint; i >= 1; i--) {
    pixelValues[i] = pixelValues[i-1];
    pixelValues[totalLength - i] = pixelValues[totalLength - i + 1];
  }

  pixelValues[0] = newValue;
  pixelValues[totalLength] = newValue;
  
  
  
  for(i=0; i< strip.numPixels(); i++) {
    // white
    strip.setPixelColor(i, strip.Color(pixelValues[i],pixelValues[i],pixelValues[i]));
    // blue
    //strip.setPixelColor(i, strip.Color(0,0,pixelValues[i]));
  }
  strip.show(); // This sends the updated pixel color to the hardware.
  /* */
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


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

