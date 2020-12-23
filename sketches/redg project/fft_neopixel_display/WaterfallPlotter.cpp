/*
  WaterfallPlotter.cpp - Library for displaying x,y,z data onto neopixel strips
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/
#include "WaterfallPlotter.h"



WaterfallPlotter::WaterfallPlotter(unsigned int width, unsigned int height, uint8_t* neopixelPins) {
  this->_width = width;
  this->_height = height;
  this->_neopixelPins = neopixelPins;
	
  // make room for the neopixels
  this->_pixelStrips = (Adafruit_NeoPixel**)malloc((height)*sizeof(Adafruit_NeoPixel*));
        
  Adafruit_NeoPixel * temp;
  for (int y = 0; y < this->_height; y++) {
    temp = new Adafruit_NeoPixel(this->_width, this->_neopixelPins[y], NEO_GRB + NEO_KHZ800);
    temp->begin();
    this->_pixelStrips[y] = temp;
  }
	
}


WaterfallPlotter::~WaterfallPlotter() {
    free(this->_pixelStrips);
    this->_pixelStrips = NULL;
}


void WaterfallPlotter::update(byte* data) {
	this->_data = data;
	
}

void WaterfallPlotter::display() {
  // logic for displaying pixels
  RgbColor color;
  
  for (unsigned int y=0; y<this->_height; y++) {
    for (unsigned int x=0; x<this->_width; x++) {
      color = this->_getPixelColor(x, y, this->_getValue(x, y));
      this->_pixelStrips[y]->setPixelColor(x, color.r, color.g, color.b);
    } 
    this->_pixelStrips[y]->show();
  }
	
}

RgbColor WaterfallPlotter::_getPixelColor(unsigned int x, unsigned int y, byte intensity) {
  float hue, saturation, value;
  // rainbow colors with red at top and purple at the bottom
  hue = 360*((float)y/this->_height);
  
  // intensity drops off towards end of graph
  value = (float)intensity / 255.0;
  value = value * (1-(float)x/this->_width);
  
  //value = ((float) rand() / (RAND_MAX)) + 1;
  
  saturation = 1;
  
  
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

