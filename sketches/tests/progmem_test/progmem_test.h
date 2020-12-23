#include "Arduino.h"

struct hsv {
    byte h;       // angle in degrees
    byte s;       // percent
    byte v;       // percent
};


struct Pixel {
  hsv fromColor;
  hsv toColor;
  unsigned int timeout; // ms
};

