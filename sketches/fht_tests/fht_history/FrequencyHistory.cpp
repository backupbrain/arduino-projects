/*
  FrequencyHistory.cpp - Library for storing x,y,z data
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/
#include "FrequencyHistory.h"

FrequencyHistory::FrequencyHistory(unsigned int width, unsigned int height, int fftResolution) {
  this->_initialize();
  this->_width = width;
  this->_height = height;
  this->_fftResolution = fftResolution;
  
  this->_data = (byte*) malloc (width*height*sizeof(byte));
}


FrequencyHistory::~FrequencyHistory() {
    free(this->_data);
    this->_data = NULL;
}


void FrequencyHistory::_initialize() {
  this->_fftFloatToByteCoefficient = 0.000001;
  this->_maxAmplitude = 0;
}

byte* FrequencyHistory::getData() {
  return this->_data;
}

int FrequencyHistory::getWidth() {
  return this->_width;
}
int FrequencyHistory::getHeight() {
  return this->_height;
}

void FrequencyHistory::loadFromFHT(byte* frequencies) {
  // scale the frequencies to our desired output
  // push onto the history
  byte values[this->_height];
  float intensity = 0;

  unsigned int initialValue = 2;
  for (int index = initialValue; index < this->_height+initialValue; index++) {
    values[index-initialValue] = frequencies[index];
  }
  
  this->_push(values);
  
}

void FrequencyHistory::scaleFromFHT(byte* frequencies) {
  // scale the frequencies to our desired output
  // push onto the history
  byte values[this->_height];
  float intensity = 0;

  for (int index = 0; index < this->_height; index++) {
    intensity = this->_scaleFrequencyIndex(frequencies, index);
    
    if (intensity > this->_maxAmplitude) this->_maxAmplitude = intensity;
  }
  
  
  //Serial.print(this->_maxAmplitude);
  //Serial.println();
  //Serial.println("--------");
  
  this->_push(values);
  
}

void FrequencyHistory::loadFromFHTOctave(byte* frequencies) {
  // scale the frequencies to our desired output
  // push onto the history.  There should only be 8 frequencies
  byte values[this->_height];

  for (int index = 0; index < this->_height; index++) {
    values[index] = frequencies[index];
  }
  
  this->_push(values);
  
}

byte FrequencyHistory::_amplitudeToByte(float intensity) {
  byte b;
  b = (byte) floor((255.0 * intensity) * this->_fftFloatToByteCoefficient);
  //b = 255;
  return b;
}

byte FrequencyHistory::_scaleFrequencyIndex(byte* frequencies, int frequencyIndex) {
  if (frequencyIndex < 1) frequencyIndex = 1;
  int minFrequencyIndex = floor(this->_width*(float)frequencyIndex/this->_fftResolution);
  if (minFrequencyIndex < 1) minFrequencyIndex = 1; // frequencies[0] is the total amplitude
  
  int maxFrequencyIndex = floor(this->_width*(float)(frequencyIndex+1)/this->_fftResolution);
  if (maxFrequencyIndex > (this->_fftResolution-1)) maxFrequencyIndex = this->_fftResolution-1;
  
  // combine the values for this scale.
  float intensity = 0;
  for (int index=minFrequencyIndex; index<maxFrequencyIndex; index++) {
    //Serial.print(frequencies[index]);
    //Serial.print(",");
     intensity += frequencies[index];
  }
  //Serial.println();
  
  intensity = intensity/(maxFrequencyIndex-minFrequencyIndex);
  
  if (intensity > this->_INTENSITY_MAX) intensity = 0; //this->_INTENSITY_MAX;
  if (intensity < this->_INTENSITY_MIN) intensity = this->_INTENSITY_MIN;
  
  intensity = round(intensity);
  
  return (byte)intensity;  
}


void FrequencyHistory::_push(byte* values) {
  // push everything in the data array over by one  
  for (unsigned int x=this->_width-1; x>0; x--) {
    for (unsigned int y=0; y<this->_height; y++) {
      this->setValue(x, y, this->getValue(x-1, y)); 
    }
  }	
  
  // add new data
  int x = 0;
  for (int y=0; y<this->_height; y++) {
    this->setValue(x, y, values[y]);
  }
  
}

byte FrequencyHistory::getValue(unsigned int x, unsigned int y) {
  return this->_data[x + y * this->_width];
}
void FrequencyHistory::setValue(unsigned int x, unsigned int y, byte value) {
  // set value at [y][x]
  // it is presumed that x=0 is the latest column of frequencies, making y the individual frequency
  this->_data[x + y * this->_width] = value;
}


