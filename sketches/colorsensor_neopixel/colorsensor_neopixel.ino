
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_TCS34725.h"


/**
 * Connect TCS3725 SCL to Analog Pin 5
 * Connect TCS3725 SDA to Analog Pin 4
 */
const int NEOPIXEL_PIN = 3;
const int NUM_NEOPIXELS = 8;


// our RGB -> eye-recognized gamma color
byte gammatable[256];

int counter = 0;

Adafruit_NeoPixel pixelStrip = Adafruit_NeoPixel(NUM_NEOPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
//Adafruit_TCS34725 tcs = Adafruit_TCS34725();

void setup() {
  Serial.begin(9600);
  
  
  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
      
      
      gammatable[i] = x;
    //Serial.println(gammatable[i]);
  }
  
  counter = 0;
  pixelStrip.begin();
}

void loop() { 
  
  uint16_t clear, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &clear);
  
  tcs.setInterrupt(false);      // turn on LED
  delay(60);  // takes 50ms to read 
  tcs.setInterrupt(true);  // turn off LED
  
  /*
  counter++;
  if (counter > 255) {
    counter = 0;
  }
  */
  
  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;
  
  
  
  /*
  Serial.print("C:\t"); Serial.print(clear);
  Serial.print("\tR:\t"); Serial.print(r);
  Serial.print("\tG:\t"); Serial.print(g);
  Serial.print("\tB:\t"); Serial.print(b);
  Serial.println();
  */
  
  
  colorWipe(pixelStrip.Color(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b]), 0);
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixelStrip.numPixels(); i++) {
      pixelStrip.setPixelColor(i, c);
      pixelStrip.show();
      delay(wait);
  }
}
