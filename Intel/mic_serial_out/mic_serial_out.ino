#include "ColorPack.h"
#include <Adafruit_NeoPixel.h>

#include "HSVAPA102.h"

const unsigned char r = 0;
const unsigned char g = 1;
const unsigned char b = 2;

const unsigned int serialBaudRate = 250000;
const unsigned int neoPixelPin = 11;
const unsigned int numPixels = 16;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(serialBaudRate);
   
  strip.begin();
  
  for (int pixel=0; pixel<strip.numPixels(); pixel++) {
    strip.setPixelColor(pixel, strip.Color(10, 10, 10));
  }
  strip.show();
}


void loop()  {



   unsigned long frame = 0; 
   byte val;
   unsigned int r, g, b;
   uint32_t color;
   uint32_t colors[strip.numPixels()];

  // let's assume for now that we will never send a 255 bit
  // 255 is reserved for clearing the data packet
  unsigned int counter = 0;

  /*
  if (Serial.available()) {

    val = Serial.read();
    strip.setPixelColor(counter, strip.Color(val, val, val));  
    counter++;
    if (counter >= strip.numPixels()) {
      counter = 0;
    }
  }
/* */

  
  for (int pixel=0; pixel<strip.numPixels(); pixel++) {
  
    
    for (int counter=0; counter<3; counter++) {
      if (Serial.available()) {
        val = Serial.read();
        
        delay(1); // Wait 10 milliseconds for next reading
        
        // if our value is 255, then let's reset the counter and begin the frame again
        if (val >= 65) {
          counter = 0;
          color = 0x000000;
        }
        if (val == 64) {
          pixel = 0;
        }
        
        if (counter == 1) {
          g = val;
        }
        if (counter == 2) {
          b = val;
        }
        if (counter == 0) {
          r = val;
        }
      } // Serial
    } // for counter
    colors[pixel] = strip.Color(r, g, b);
  } // for pixel

  // what do we do with this value?

  for (int pixel=0; pixel<strip.numPixels(); pixel++) {
    strip.setPixelColor(pixel, colors[pixel]);
  }
  /* */
  strip.show();
   
}

