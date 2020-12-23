/*
  InfinityLights.cpp - Library for displaying x,y,z data onto neopixel strips
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/
#include "InfinityLights.h"



InfinityLights::InfinityLights(unsigned int numPixels, uint8_t dataPin) {
  this->_dataPin = dataPin;
  this->_numPixels = numPixels;
	
  
  // make room for the neopixels
  this->_pixelStrip = (SimpleNeopixel*)malloc((numPixels)*sizeof(SimpleNeopixel));
  unsigned int halfPoint = floor(this->_numPixels/2);
  this->_lightBrigtnesses = (byte*)malloc((halfPoint)*sizeof(byte));
  
  this->_pixelStrip = new SimpleNeopixel(numPixels, this->_dataPin, NEO_GRB + NEO_KHZ800);	
  this->_pixelStrip->begin();
}


InfinityLights::~InfinityLights() {
    free(this->_pixelStrip);
    this->_pixelStrip = NULL;
    free(this->_lightBrigtnesses);
    this->_lightBrigtnesses = NULL;
}



void InfinityLights::update(byte newValue) {
  // push new values onto the array
  unsigned int halfPoint = floor(this->_numPixels/2);
  for (int index=halfPoint-1; index>0; index--) {
    this->_lightBrigtnesses[index] = this->_lightBrigtnesses[index-1];
  }
  this->_lightBrigtnesses[0] = newValue;  
}

void InfinityLights::display() {  
  // this is how we do it with the SimpleNeopixel library
  unsigned int halfPoint = floor(this->_numPixels/2);
  unsigned int invertedIndex = this->_numPixels;
  /*
  byte invertedIndex[halfPoint];
  for (int index=0; index < halfPoint; index++) {
    invertedIndex[index] = halfPoint-index-1;
  }
  /* */
  cli();
  for (int index=0; index<halfPoint; index++) {
    this->_pixelStrip->sendPixel(this->_lightBrigtnesses[index], this->_lightBrigtnesses[index], this->_lightBrigtnesses[index]);
    //this->_pixelStrip->sendPixel(255,0,0);
  }
  for (int index=0; index<halfPoint; index++) {
    invertedIndex = halfPoint-index-1;
    this->_pixelStrip->sendPixel(this->_lightBrigtnesses[invertedIndex], this->_lightBrigtnesses[invertedIndex], this->_lightBrigtnesses[invertedIndex]);
    //this->_pixelStrip->sendPixel(255,0,0);
  }
  /* */
  sei();
  this->_pixelStrip->show();
  /* */
  
}


