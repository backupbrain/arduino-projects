
#include "Image.h"

Image::Image(unsigned int width, unsigned int height, byte* imageData) {
  this->_width = width;
  this->_height = height;
  this->_imageData = imageData;
}


Image::~Image() {
}
    
unsigned int Image::getWidth() {
  return this->_width;
}      
unsigned int Image::getHeight() {
  return this->_height;
}
byte Image::getPixelAt(unsigned int x, unsigned int y) {
  unsigned int pixelOffset = (x * this->_width) + y;  
  
  return this->_imageData[pixelOffset];
}
    
unsigned char* Image::getImageData() {
  return this->_imageData;
}
