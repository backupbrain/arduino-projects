/*
  FrequencyHistory.cpp - Library for storing x,y,z data
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/

#ifndef FrequencyHistory_h
#define FrequencyHistory_h

#include "Arduino.h"


class FrequencyHistory
{
  public:    
    FrequencyHistory(unsigned int width, unsigned int height, int fftResolution);
    ~FrequencyHistory();  
    
    int getWidth();
    int getHeight();
    byte getValue(unsigned int x, unsigned int y);
    void setValue(unsigned int x, unsigned int y, byte value);
	
    void loadFromFHT(byte* values);
    void scaleFromFHT(byte* values);
    void loadFromFHTOctave(byte* values);
    byte* getData();

  private:
    static const unsigned int _INTENSITY_MAX = 200000;
    static const unsigned int _INTENSITY_MIN = 0;
    unsigned int _width;
    unsigned int _height;
    byte* _data;
    float _fftFloatToByteCoefficient; // = 0.000001; // 1/100000;
    float _fftResolution;
    float _maxAmplitude; // = 0;
    void _initialize();
    void _push(byte* values);
    byte _amplitudeToByte(float amplitude);
    byte _scaleFrequencyIndex(byte* frequencies, int frequencyIndex);
    
};

#endif

