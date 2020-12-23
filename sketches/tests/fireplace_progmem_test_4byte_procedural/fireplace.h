#include "Arduino.h"
#include "color.h"
#include "neopixel.h"

struct Pixel {
  hsv color;
  unsigned int timeout; // ms
};


struct PixelPointer {
  byte fromPixelIndex;
  byte toPixelIndex;
  unsigned int timeout;
};


