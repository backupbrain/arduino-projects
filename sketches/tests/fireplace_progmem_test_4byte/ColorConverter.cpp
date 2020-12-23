/*
  ColorConverter.cpp - Library for converting between color space
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/
#include "ColorConverter.h"

/**
 * convert hsv color space to rgb color space
 * based on code from http://stackoverflow.com/a/22120275
 * 
 * @param hsvColor an hsv color struct
 * @return an rgb color struct
 */
rgb ColorConverter::hsv2rgb(hsv hsvColor) {
    rgb rgbColor;
    unsigned char region, p, q, t;
    unsigned int h, s, v, remainder;

    if (hsvColor.s == 0)
    {
        rgbColor.r = hsvColor.v;
        rgbColor.g = hsvColor.v;
        rgbColor.b = hsvColor.v;
        return rgbColor;
    }

    // converting to 16 bit to prevent overflow
    h = hsvColor.h;
    s = hsvColor.s;
    v = hsvColor.v;

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgbColor.r = v;
            rgbColor.g = t;
            rgbColor.b = p;
            break;
        case 1:
            rgbColor.r = q;
            rgbColor.g = v;
            rgbColor.b = p;
            break;
        case 2:
            rgbColor.r = p;
            rgbColor.g = v;
            rgbColor.b = t;
            break;
        case 3:
            rgbColor.r = p;
            rgbColor.g = q;
            rgbColor.b = v;
            break;
        case 4:
            rgbColor.r = t;
            rgbColor.g = p;
            rgbColor.b = v;
            break;
        default:
            rgbColor.r = v;
            rgbColor.g = p;
            rgbColor.b = q;
            break;
    }

    return rgbColor;
}



/**
 * convert rgb color space to hsv color space
 * 
 * @param rgbColor an rgb color struct
 * @return an hsv color struct
 */
hsv ColorConverter::rgb2hsv(rgb rgbColor)
{
    hsv hsvColor;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgbColor.r < rgbColor.g ? (rgbColor.r < rgbColor.b ? rgbColor.r : rgbColor.b) : (rgbColor.g < rgbColor.b ? rgbColor.g : rgbColor.b);
    rgbMax = rgbColor.r > rgbColor.g ? (rgbColor.r > rgbColor.b ? rgbColor.r : rgbColor.b) : (rgbColor.g > rgbColor.b ? rgbColor.g : rgbColor.b);

    hsvColor.v = rgbMax;
    if (hsvColor.v == 0)
    {
        hsvColor.h = 0;
        hsvColor.s = 0;
        return hsvColor;
    }

    hsvColor.s = 255 * ((long)(rgbMax - rgbMin)) / hsvColor.v;
    if (hsvColor.s == 0)
    {
        hsvColor.h = 0;
        return hsvColor;
    }

    if (rgbMax == rgbColor.r)
        hsvColor.h = 0 + 43 * (rgbColor.g - rgbColor.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgbColor.g)
        hsvColor.h = 85 + 43 * (rgbColor.b - rgbColor.r) / (rgbMax - rgbMin);
    else
        hsvColor.h = 171 + 43 * (rgbColor.r - rgbColor.g) / (rgbMax - rgbMin);

    return hsvColor;
}
