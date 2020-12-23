
#include "Simple_Neopixel.h"

const byte ledPin = 13;

const byte neopixelWidth = 8;
const byte neopixelHeight = 8;
const byte neopixelPin = 6;


Simple_Neopixel pixelStrip = Simple_Neopixel(neopixelWidth*neopixelHeight, neopixelPin, NEO_GRB + NEO_KHZ800);

void setup()
{
    pinMode(ledPin, OUTPUT);
	pixelStrip.begin();
}

void loop()
{
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(900);



  cli();
  
  for (int x = 0; x < neopixelWidth; x++) {
  	for (int y = 0; y < neopixelHeight; y++) {
    //this->_pixelStrips[pinID]->setPixelColor(pixel, pixelColors[pinID][pixel].r, pixelColors[pinID][pixel].g, pixelColors[pinID][pixel].b);
    //pixelStrip->sendPixel(pixelColors[pixel].r, pixelColors[pixel].g, pixelColors[pixel].b);
    	pixelStrip.sendPixel(64,0,0);
	}
  }
    
  sei(); 
  pixelStrip.show();
}
