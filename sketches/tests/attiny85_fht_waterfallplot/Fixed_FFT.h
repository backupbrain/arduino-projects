

#ifndef Fixed_FFT_h
#define Fixed_FFT_h

#include <avr/pgmspace.h>
#include "Arduino.h"


class Fixed_FFT {
  public:
    static int fft(char* fr, char* fi, int m, int inverse);  
    static int fftr(char* f, int m, int inverse);
};

#endif
