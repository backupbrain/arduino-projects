#include "Arduino.h"
#include "ColorConverter.h"

struct Pixel {
  hsv color;
  unsigned int timeout; // ms
};


struct PixelPointer {
  byte fromPixelIndex;
  byte toPixelIndex;
  unsigned int timeout;
};
