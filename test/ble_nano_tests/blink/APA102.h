/**
 * APA102 Parsing/Displaying class
 * 
 * This class takes in RGB color data and converts it to APA102 binary data,
 * and retrieves APA102 binary data and builds an RGB color table
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-26
 */

#include <Arduino.h>

struct RGBColor {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};



class APA102 {

  public:
    APA102(uint8_t numPixels);
    ~APA102();
    
    void resetReadCarat();
    void resetWriteCarat();
    
    void putAPA102(uint32_t packet);
    uint32_t getAPA102(uint8_t index);
    

    void putRGBColor(RGBColor color);
    RGBColor getRGBColor(uint8_t index);
    
    uint32_t RGB2APA102(RGBColor color);
    RGBColor APA1022RGB(uint32_t apa102);


  private:
    uint8_t _numPixels;
    RGBColor* _colors;
    uint8_t _readCarat;
    uint8_t _writeCarat;
  
};

 
