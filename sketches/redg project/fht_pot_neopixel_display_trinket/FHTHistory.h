/*
  FHTHistory.cpp - Library for storing x,y,z data
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/

#ifndef FHTHistory_h
#define FHTHistory_h

#include "Arduino.h"


class FHTHistory
{
  public:    
    FHTHistory(unsigned int width, unsigned int height, unsigned int fftResolution);
    ~FHTHistory();  
    
    unsigned int getWidth();
    unsigned int getHeight();
    byte getValue(unsigned int x, unsigned int y);
    void setValue(unsigned int x, unsigned int y, byte value);
	
    void loadFromFHT(byte* values);
    byte* getData();

  private:
    unsigned int _width;
    unsigned int _height;
    byte* _data;
    float _fhtResolution;
    void _push(byte* values);
    
};

#endif

