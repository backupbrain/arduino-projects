

unsigned int* getColor(uint32_t hexColor) {
  static unsigned int rgbColor[3];
  int r = hexColor >> 16;
  int g = hexColor >> 8 & 0xFF;
  int b = hexColor & 0xFF;

  rgbColor[0] = r;
  rgbColor[1] = g;
  rgbColor[2] = b;
  
  return rgbColor;
}

