#include <Arduino.h>

/*
class NrfNeopixel {
  public:
    NrfNeopixel(byte p, unsigned int n);
    ~NrfNeopixel();

    void init();
    void clear();
    void show();
    void setPixelColor(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);
  private:
    uint8_t _pin;
    uint8_t _numLEDs;
    uint32_t *_pixels;
};

/* */



class NrfNeopixel {
  public:
    NrfNeopixel (uint8_t pin, uint32_t numPixels);
    ~NrfNeopixel();

    void 
      init(),
      clear(),
      show(),
      setPixelColor(uint32_t index, uint8_t red, uint8_t green, uint8_t blue),
      setPixelColor(uint32_t index, uint32_t rgb);
   uint32_t
      getColor(uint8_t r, uint8_t g, uint8_t b);
    uint8_t 
      numPixels() { return this->_numPixels; }
  private:
    void
      _sendOneBit(),
      _sendZeroBit();
    uint8_t 
      _pin,
      _numPixels;
    uint8_t**
      _pixels;
};
