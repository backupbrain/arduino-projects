/*
  WaterfallPlotter.cpp - Library for displaying x,y,z data onto neopixel strips
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/
#include "WaterfallPlotter.h"



WaterfallPlotter::WaterfallPlotter(unsigned int width, unsigned int height, uint8_t* neopixelPins, uint8_t numPins) {
  this->_maxValue = 1.0;
  this->_width = width;
  this->_height = height;
  this->_neopixelPins = neopixelPins;
  this->_numPins = numPins;
	
  // make room for the neopixels
  this->_pixelStrips = (Adafruit_NeoPixel**)malloc((height)*sizeof(Adafruit_NeoPixel*));
  
  for (int pinID = 0; pinID < this->_numPins; pinID++) {
    this->_pixelStrips[pinID] = new Adafruit_NeoPixel(this->_width*3, this->_neopixelPins[pinID], NEO_GRB + NEO_KHZ800);
    this->_pixelStrips[pinID]->begin();
  }
	
}


WaterfallPlotter::~WaterfallPlotter() {
    free(this->_pixelStrips);
    this->_pixelStrips = NULL;
}


void WaterfallPlotter::setMaxValue(float maxValue) {
  if (maxValue < 0) maxValue = 0;
  if (maxValue > 1) maxValue = 1;
  this->_maxValue = maxValue;
}
float WaterfallPlotter::getMaxValue() {
  return this->_maxValue;
}

void WaterfallPlotter::update(byte* data) {
	this->_data = data;
	
}

void WaterfallPlotter::display() {
  // logic for displaying pixels
  RgbColor color;
  
  // we are putting two frequency bands onto a single pixel strip 
  // we need to arrange pixels like this:
  /*
   * [   strip 1   ] [   strip 2  ]
   *   0 1 2 3 4 5  -  5 4 3 2 1 0
   * DATA  -> [ 5 4 3 2 1 0 ]   0
   *                         3 
   * DRAIN <- [ 5 4 3 2 1 0 ]   1
   */
   
  RgbColor pixelColors[this->_numPins][this->_width*2];
  
  unsigned int y = 0;
  unsigned int x = 0;
  int maxPixels = this->_width*2;
  int minPixels = 0;
  for (int pinID = 0; pinID < this->_numPins; pinID++) {
    for (int pixel = 0; pixel < this->_width; pixel++) {
//    for (int pixel = (this->_width-1); pixel >= 0; pixel--) {
      x = this->_width - pixel - 1;
//      x = this->_width - pixel - 1;
      pixelColors[pinID][pixel] = this->_getPixelColor(x, y, this->_getValue(x, y));
      //this->_pixelStrips[pinID]->setPixelColor(pixel, color.r, color.g, color.b);
      //pixelColors[pinID][pixel] = color;
      Serial.print("x: ");
      Serial.print(x);
      Serial.print(", y: ");
      Serial.print(y);
      Serial.print(", pin: ");
      Serial.print(this->_neopixelPins[pinID]);
      Serial.print(", pixel: ");
      Serial.print(pixel);
      Serial.println();
    }
      
    y++;
    
    // work away from data source for second strip
    for (int pixel = this->_width; pixel < maxPixels; pixel++) {
      x = pixel - this->_width;
//      x = maxPixels-pixel-1;
      pixelColors[pinID][pixel] = this->_getPixelColor(x, y, this->_getValue(x, y));
      //pixelColors[pinID] = color;
      Serial.print("x: ");
      Serial.print(x);
      Serial.print(", y: ");
      Serial.print(y);
      Serial.print(", pin: ");
      Serial.print(this->_neopixelPins[pinID]);
      Serial.print(", pixel: ");
      Serial.print(pixel);
      Serial.println();
    }
    
    
    
    y++;
    
    maxPixels = this->_width*3;
    minPixels = this->_width*2;
    // work away from data source for second strip
    for (int pixel = maxPixels-1; pixel >= minPixels; pixel--) {
      x = pixel - minPixels;
//      x = maxPixels-pixel-1;
      pixelColors[pinID][pixel] = this->_getPixelColor(x, y, this->_getValue(x, y));
      //pixelColors[pinID] = color;
      Serial.print("x: ");
      Serial.print(x);
      Serial.print(", y: ");
      Serial.print(y);
      Serial.print(", pin: ");
      Serial.print(this->_neopixelPins[pinID]);
      Serial.print(", pixel: ");
      Serial.print(pixel);
      Serial.println();
    }
    
    
    
    y++;
  }
  
  
  
  for (int pinID = 0; pinID < this->_numPins; pinID++) {
    for (int pixel = 0; pixel < maxPixels; pixel++) {
      this->_pixelStrips[pinID]->setPixelColor(pixel, pixelColors[pinID][pixel].r, pixelColors[pinID][pixel].g, pixelColors[pinID][pixel].b);
    }
    
    cli();
    this->_pixelStrips[pinID]->show();
    sei();
  }
 
  /* */
  
  /*
  // if we had each frequency on it's own LED strip, we would use this loop
  for (int y=0; y<this->_height; y++) {
    for (int x=0; x<this->_width; x++) {
      color = this->_getPixelColor(x, y, this->_getValue(x, y));
      this->_pixelStrips[y]->setPixelColor(x, color.r, color.g, color.b);
    } 
    this->_pixelStrips[y]->show();
  }
  /* */
	
}

RgbColor WaterfallPlotter::_getPixelColor(unsigned int x, unsigned int y, byte intensity) {
  float hue, saturation, value;
  // rainbow colors with red at top and purple at the bottom
  hue = 360*((float)y/this->_height);
  
  // intensity drops off towards end of graph
  value = (float)intensity / 255.0; 
  value = value * (1-(float)x/this->_width);
  value = value * this->_maxValue;
  
  //value = ((float) rand() / (RAND_MAX)) + 1;
  
  //saturation = 1;
  
  saturation = 1 - ((float)intensity / (255.0/2)); 
  
  
  //hue = 20;
  //saturation = 1;
  //value = 0.2;
  
  HsvColor hsv;
//  hsv.h = (unsigned char) floor(255*hue/360);
  hsv.h = (unsigned char) floor(30);
  hsv.s = (unsigned char) floor(255*saturation);
  hsv.v = (unsigned char) floor(255*value);
  
  //Serial.print("value: ");
  //Serial.println(hsv.v);
  return ColorConverter::HsvToRgb(hsv);
  
}



int WaterfallPlotter::_getValue(unsigned int x, unsigned int y) {
  return this->_data[x + y * this->_width];
}

