/*
  AudioSampler.cpp -  Library for displaying equalizer on a NeoPixel screen
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.
*/
#include "PixelEqualizer.h"

PixelEqualizer::PixelEqualizer(unsigned int neoPixelPin, unsigned int xResolution, unsigned int yResolution) {  
  _neoPixelPin = neoPixelPin;
  _xRes = xResolution;
  _yRes = yResolution;  
  
  hues = (float*)malloc(xResolution*sizeof(float));
  
  
  // Evenly spread hues across all pixels.
  for (int i = 0; i < xResolution; ++i) {
    hues[i] = 360.0*(float(i)/float(xResolution-1));
  }
  
  unsigned int numNeoPixels = xResolution * yResolution;
  pixelStrip = new Adafruit_NeoPixel(numNeoPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);
  pixelStrip->begin();
  
  
}

PixelEqualizer::~PixelEqualizer() {
}

void PixelEqualizer::display(float* spectrum) {
  
  float intensity = 0;
  for (int i=0; i<_yRes; i++) {
    intensity = spectrum[i];
    
    
    // Scale the intensity and clamp between 0 and 1.0.
    intensity -= SPECTRUM_MIN_DB;
    intensity = intensity < 0.0 ? 0.0 : intensity;
    intensity /= (SPECTRUM_MAX_DB-SPECTRUM_MIN_DB);
    intensity = intensity > 1.0 ? 1.0 : intensity;
  
  
    Serial.print(intensity);
    Serial.print(",");
 
  int allPixels = _xRes*_yRes;
  
      //pixelStrip.setPixelColor(i,  pixelStrip.Color(intensity*255, 0, 0));
      
    //int numPixelsTall = 8;
    int numPixelsTall = intensity * _yRes;
    
    unsigned int pixelPosition = 0;
    for (int y=0; y<allPixels; y++) {
      pixelPosition = (y*8)+i;
      if (y <= numPixelsTall) {
        pixelStrip->setPixelColor(pixelPosition, _pixelHSVtoRGBColor(hues[i], 1.0, intensity));
      } else {
        pixelStrip->setPixelColor(pixelPosition,  pixelStrip->Color(0, 0, 0));
      }
    } 
  
    
    
  }
  
  
  Serial.print("\n---------------\n");
    
  pixelStrip->show(); 
}



// Convert from HSV values (in floating point 0 to 1.0) to RGB colors usable
// by neo pixel functions.
uint32_t PixelEqualizer::_pixelHSVtoRGBColor(float hue, float saturation, float value) {
  // Implemented from algorithm at http://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV
  float chroma = value * saturation;
  float h1 = float(hue)/60.0;
  float x = chroma*(1.0-fabs(fmod(h1, 2.0)-1.0));
  float r = 0;
  float g = 0;
  float b = 0;
  if (h1 < 1.0) {
    r = chroma;
    g = x;
  }
  else if (h1 < 2.0) {
    r = x;
    g = chroma;
  }
  else if (h1 < 3.0) {
    g = chroma;
    b = x;
  }
  else if (h1 < 4.0) {
    g = x;
    b = chroma;
  }
  else if (h1 < 5.0) {
    r = x;
    b = chroma;
  }
  else // h1 <= 6.0
  {
    r = chroma;
    b = x;
  }
  float m = value - chroma;
  r += m;
  g += m;
  b += m;
  return pixelStrip->Color(int(255*r), int(255*g), int(255*b));
}

