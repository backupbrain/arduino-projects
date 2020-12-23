/*
  WaterfallPlotter.cpp - Library for displaying x,y,z data onto neopixel strips
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/
#include "WaterfallPlotter.h"



WaterfallPlotter::WaterfallPlotter(unsigned int width, unsigned int height, uint8_t neopixelPin) {
  this->_maxValue = 1.0;
  this->_width = width;
  this->_height = height;
  this->_neopixelPin = neopixelPin;
	
  // make room for the neopixels
  this->_pixelStrip = (Simple_Neopixel*)malloc((height)*sizeof(Simple_Neopixel));
  
  this->_pixelStrip = new Simple_Neopixel(this->_width*this->_height, this->_neopixelPin, NEO_GRB + NEO_KHZ800);
  this->_pixelStrip->begin();
	
}


WaterfallPlotter::~WaterfallPlotter() {
    free(this->_pixelStrip);
    this->_pixelStrip = NULL;
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
   
  RgbColor pixelColors[this->_width*this->_height];
  
  boolean DIRECTION_REVERSE = 0;
  boolean DIRECTION_FORWARD = 1;
  
  int y = 0;
  int x = 0;
  unsigned int maxPixels = 0;
  unsigned int minPixels = 0;
  boolean animationDirection = DIRECTION_REVERSE;
  for (int row = 0; row < this->_height; row++) {
    //for (int pinID = 0; pinID < this->_numPins; pinID++) {
      maxPixels = this->_width*(row+1);
      minPixels = this->_width*row;
      // work away from data source for second strip
      for (int pixel = minPixels; pixel < maxPixels; pixel++) {
        x = 0;
        if (animationDirection == DIRECTION_REVERSE) {
            x = maxPixels - pixel - 1;
        } else {
            //x = pixel - minPixels;
            x = maxPixels - pixel - 1; // single direction test mode
        }
        
        color = this->_getPixelColor(x, y, this->_getValue(x, y));
        pixelColors[pixel] = color;
        /*
        Serial.print(" x: ");
        Serial.print(x);
        Serial.print(" y: ");
        Serial.print(y);
        Serial.print(" pixel: ");
        Serial.print(pixel);
        Serial.println();
        /* */
      }
      y++;
      animationDirection = !animationDirection;
      //Serial.println("-------------");
    //}
    
  }
  //Serial.println("==============");
  
  
  cli();
  
  for (int pixel = 0; pixel < maxPixels; pixel++) {
    //this->_pixelStrips[pinID]->setPixelColor(pixel, pixelColors[pinID][pixel].r, pixelColors[pinID][pixel].g, pixelColors[pinID][pixel].b);
    this->_pixelStrip->sendPixel(pixelColors[pixel].r, pixelColors[pixel].g, pixelColors[pixel].b);
    //this->_pixelStrip->sendPixel(64,0,0);
  }
    
  sei(); 
  this->_pixelStrip->show();
	
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
  
  //hsv.v = hsv.v/2 + floor((float)hsv.v * ((float)this->_image->getPixelAt(x, y)/255))/2; 
  //hsv.v = hsv.v * (float)this->_image->getPixelAt(x, y)/255;
  //hsv.v = this->_image->getPixelAt(x, y); // test the image
  
  //Serial.print("value: ");
  //Serial.println(hsv.v);
  return ColorConverter::HsvToRgb(hsv);
  
}


int WaterfallPlotter::_getValue(unsigned int x, unsigned int y) {
  return this->_data[x + y * this->_width];
}

