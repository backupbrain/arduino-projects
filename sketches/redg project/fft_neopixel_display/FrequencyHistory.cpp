/*
  FrequencyHistory.cpp - Library for storing x,y,z data
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/
#include "FrequencyHistory.h"

FrequencyHistory::FrequencyHistory(unsigned int width, unsigned int height, int fftResolution) {
  this->_width = width;
  this->_height = height;
  this->_fftResolution = fftResolution;
  
  

  this->_exponentialToLinearFrequencyCoefficient = (float)log(this->_fftResolution)/this->_height;
  this->_data = (byte*) malloc (width*height*sizeof(byte));
}


FrequencyHistory::~FrequencyHistory() {
    free(this->_data);
    this->_data = NULL;
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

void FrequencyHistory::loadFromFFT(float* frequencies) {
  // convert exponential frequency distribution to a linear one
  // then convert everything to bytes to save memory
  byte values[this->_height];
  float intensity = 0;

  Serial.println();
  Serial.println("--------");

  for (int index = 0; index < this->_height; index++) {
    intensity = this->_frequencyIndexToNaturalIndex(frequencies, index);
    //intensity = this->_toDecibels(amplitude);
    //intensity = rand();
    
    if (intensity > this->_maxAmplitude) this->_maxAmplitude = intensity;
    values[index] = this->_amplitudeToByte(intensity);
    
    
    //Serial.print(values[index]);
    //Serial.print(",");
  }
  
  if (this->_maxAmplitude != 0) {
    this->_fftFloatToByteCoefficient = (1/this->_maxAmplitude);
  }
  
  
  Serial.print(this->_maxAmplitude);
  Serial.println();
  Serial.println("--------");
  
  this->_push(values);
}

byte FrequencyHistory::_amplitudeToByte(float intensity) {
  byte b;
  b = (byte) floor((255.0 * intensity) * this->_fftFloatToByteCoefficient);
  //b = 255;
  return b;
}

float FrequencyHistory::_frequencyIndexToNaturalIndex(float* frequencies, int frequencyIndex) {
  int minFrequencyIndex = (int)floor(exp(this->_exponentialToLinearFrequencyCoefficient*frequencyIndex));
  if (minFrequencyIndex < 1) minFrequencyIndex = 1; // frequencies[0] is the total amplitude
  
  int maxFrequencyIndex = (int)floor(exp(this->_exponentialToLinearFrequencyCoefficient*(frequencyIndex+1)));
  if (maxFrequencyIndex > (this->_fftResolution-1)) maxFrequencyIndex = this->_fftResolution-1;
  
  // combine the values for this scale.
  float intensity = 0;
  for (int index=minFrequencyIndex; index<maxFrequencyIndex; index++) {
    Serial.print(frequencies[index]);
    Serial.print(",");
     intensity += frequencies[index];
  }
  Serial.println();
  
  intensity = intensity/(maxFrequencyIndex-minFrequencyIndex);
  
  if (intensity > this->_INTENSITY_MAX) intensity = 0; //this->_INTENSITY_MAX;
  if (intensity < this->_INTENSITY_MIN) intensity = this->_INTENSITY_MIN;
  
  return intensity;  
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

int FrequencyHistory::getValue(unsigned int x, unsigned int y) {
  return this->_data[x + y * this->_width];
}
void FrequencyHistory::setValue(unsigned int x, unsigned int y, byte value) {
  this->_data[x + y * this->_width] = value;
}


