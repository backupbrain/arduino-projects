/**
 * RGB Color
 * 
 * This structure represents RGB Color values
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-28
 */


#include "RGBColor.h"


uint32_t RGBColor::RGB2APA102(RGBColor* color) {
  // begin with zeros
  uint32_t apa102 = 0xff << 24;
  // pack in as RGB
  apa102 |= color->red << 16;
  apa102 |= color->green << 8;
  apa102 |= color->blue;

  return apa102;
}
RGBColor RGBColor::APA1022RGB(uint32_t apa102) {
  // unpack as RGB
  RGBColor color;
  color.red = (apa102 >> 16) & 0xff;
  color.green = (apa102 >> 8) & 0xff;
  color.blue = apa102 & 0xff;
  return color;
}



void RGBColor::printRGB(RGBColor* color) { 
  fprintf(stdout, "%02X%02X%02X", color->red, color->green, color->blue);
}



void RGBColor::printAPA102(uint32_t apa102) {
  fprintf(stdout, "%08X", apa102);
}
