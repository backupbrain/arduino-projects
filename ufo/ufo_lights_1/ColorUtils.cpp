#include "ColorUtils.h"

/**
 * Correct the LED brightness for human perception
 */
unsigned int gammaCorrect(const unsigned int brightness) {
  return pgm_read_byte(&gamma8[brightness]);
}

/**
 * Convert Hue, Saturation, Value color into Red, Green, Blue color
 */
RgbColor HsvToRgb(HsvColor hsv) {
    RgbColor rgb;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0) {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6; 

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

/**
 * Average several HSV colors into a single color.
 */
HsvColor averageHues(HsvColor* hues, unsigned int numColors) {
  HsvColor averageHsv = HsvColor();
  averageHsv.s = 255;
  float x = 0.0;
  float y = 0.0;
  unsigned int magnitudeS = 0;
  unsigned int magnitudeV = 0;  
  for (int color = 0; color < numColors; color++) {
    magnitudeV += hues[color].v;
    magnitudeS += hues[color].s;
    x += cos(360.0 * hues[color].h / 255.0);
    y += sin(360.0 * hues[color].h / 255.0);
  }
  averageHsv.h = 255 * atan2(x, y);
  averageHsv.s = (int) magnitudeS / numColors;
  averageHsv.v = (int) magnitudeV / numColors;
  return averageHsv;
}

