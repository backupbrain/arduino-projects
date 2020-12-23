#include "NrfNeopixel.h"


NrfNeopixel::NrfNeopixel(uint8_t pin, uint32_t numPixels): _pin(pin), _numPixels(numPixels) {
  pinMode(this->_pin, OUTPUT);
  //this->_pixels = (uint8_t *)malloc(sizeof(uint8_t)*numPixels*3);  
  //this->_pixels = 3*malloc( numPixels * sizeof( uint8_t* ));

  this->_pixels = (uint8_t**)malloc(sizeof(uint8_t**) * this->_numPixels);

  
  for (int i = 0; i < this->_numPixels; i++) {  
    this->_pixels[i] = (uint8_t*)malloc(3*sizeof(uint8_t*));
  }
  /* */
  
  for (int i = 0; i < this->_numPixels; i++) {  
    this->setPixelColor(i, 0, 0, 0);
  }
  

}


NrfNeopixel::~NrfNeopixel() {
  free(this->_pixels);
  this->_pixels = NULL;
}


void NrfNeopixel::init() {
  
  for (int i = 0; i < this->_numPixels; i++) {  
    this->setPixelColor(i, 0, 0, 0);
  }

  this->show();
}
void NrfNeopixel::clear() {
  for (int i = 0; i < this->_numPixels; i++) {  
    this->setPixelColor(i, 0, 0, 0);
  }

  this->show();
}
void NrfNeopixel::show() {
  uint8_t numColors = 3;
  NRF_GPIO->OUTCLR = (1UL << this->_pin);
  delay(50);
  for (int pixel=0; pixel<this->_numPixels; pixel++) {
    for (int color=0; color<numColors; color++) {
  
      if ((this->_pixels[pixel][color] & 0x80) > 0)  {this->_sendOneBit();}
      else    {this->_sendZeroBit();}
      
      if ((this->_pixels[pixel][color] & 0x40) > 0)   {this->_sendOneBit();}
      else    {this->_sendZeroBit();}
      
      if ((this->_pixels[pixel][color] & 0x20) > 0)   {this->_sendOneBit();}
      else    {this->_sendZeroBit();}
      
      if ((this->_pixels[pixel][color] & 0x10) > 0)   {this->_sendOneBit();}
      else    {this->_sendZeroBit();}
      
      if ((this->_pixels[pixel][color] & 0x08) > 0)    {this->_sendOneBit();}
      else    {this->_sendZeroBit();}
      
      if ((this->_pixels[pixel][color] & 0x04) > 0)    {this->_sendOneBit();}
      else    {this->_sendZeroBit();}
      
      if ((this->_pixels[pixel][color] & 0x02) > 0)    {this->_sendOneBit();}
      else    {this->_sendZeroBit();}
      
      if ((this->_pixels[pixel][color] & 0x01) > 0)    {this->_sendOneBit();}
      else    {this->_sendZeroBit();}
    }
  }
  
}
void NrfNeopixel::setPixelColor(uint32_t index, uint8_t red, uint8_t green, uint8_t blue) {
  this->_pixels[index][0] = red;
  this->_pixels[index][1] = green;
  this->_pixels[index][2] = blue;
}



void NrfNeopixel::setPixelColor(uint32_t index, uint32_t color) {
  uint8_t r, g, b;
  b = color & 0xFF;
  g = (color >> 8) & 0xFF;
  r = (color >> 16) & 0xFF;
  this->setPixelColor(index, r, g, b);
  
}

uint32_t NrfNeopixel::getColor(uint8_t r, uint8_t g, uint8_t b) {
  uint32_t rgb = r;
  rgb = (rgb << 8) + g;
  rgb = (rgb << 8) + b;
  return rgb;
}


void NrfNeopixel::_sendOneBit() {
  NRF_GPIO->OUTSET = (1UL << this->_pin);
  NRF_GPIO->OUTSET = (1UL << this->_pin);
  NRF_GPIO->OUTSET = (1UL << this->_pin);
  NRF_GPIO->OUTCLR = (1UL << this->_pin);
}

void NrfNeopixel::_sendZeroBit() {
  NRF_GPIO->OUTSET = (1UL << this->_pin);
  NRF_GPIO->OUTCLR = (1UL << this->_pin);
  NRF_GPIO->OUTCLR = (1UL << this->_pin);
  NRF_GPIO->OUTCLR = (1UL << this->_pin);
  NRF_GPIO->OUTCLR = (1UL << this->_pin);
}

/* */
