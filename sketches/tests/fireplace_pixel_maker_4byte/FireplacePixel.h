#include "Arduino.h"

struct hsv {
    byte h;       // angle in degrees
    byte s;       // percent
    byte v;       // percent
};


struct Pixel {
  hsv color;
  unsigned int timeout; // ms
};



