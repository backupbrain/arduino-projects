/*
  FHTChannelHistory.cpp - Library for storing a single channel frequency volume changes over time
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/

#ifndef FHTChannelHistory_h
#define FHTChannelHistory_h

#include "Arduino.h"


class FHTChannelHistory
{
  public:    
    FHTChannelHistory(unsigned int historyBufferLength);
    ~FHTChannelHistory();  
    
    unsigned int getHistoryBufferLength();
    int getValue(unsigned int historyPosition);
    void setValue(unsigned int historyPosition, byte value);
	
    void loadFromFHT(byte value);
    byte* getData();

  private:
    unsigned int _historyBufferLength;
    byte* _data;
    
};

#endif

