/*
 
Copyright (c) 2012-2014 RedBearLab
 
Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
*/
 
#ifndef _SPI_MASTER_H_
#define _SPI_MASTER_H_
 
#include "mbed.h"
 
#define SPI_FREQUENCY_125K 0x02000000UL
#define SPI_FREQUENCY_250K 0x04000000UL
#define SPI_FREQUENCY_500K 0x08000000UL
#define SPI_FREQUENCY_1M   0x10000000UL
#define SPI_FREQUENCY_2M   0x20000000UL
#define SPI_FREQUENCY_4M   0x40000000UL
#define SPI_FREQUENCY_8M   0x80000000UL
 
#define SPI_125K 0
#define SPI_250K 1
#define SPI_500K 2
#define SPI_1M   3
#define SPI_2M   4
#define SPI_4M   5
#define SPI_8M   6
 
#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3
 
#define CS      P0_10
#define SCK     P0_8
#define MOSI    P0_9
#define MISO    P0_11


typedef enum{
    
    msbfirst = 0,
    lsbfirst = 1
    
}bitorder;
/* */
 
class SPIClass
{
    public:
        SPIClass(NRF_SPI_Type *_spi);
    
        void begin();
        void begin(uint32_t sck, uint32_t mosi, uint32_t miso);
        uint8_t transfer(uint8_t data);
        void endTransfer();
            
        void setSPIMode( uint8_t mode );
        void setFrequency(uint8_t speed );
        void setBitORDER( bitorder  bit);
        void setCPHA( bool trailing);
        void setCPOL( bool active_low);
        
        
    private:
        NRF_SPI_Type *spi;
        
        uint32_t SCK_Pin;
        uint32_t MOSI_Pin;
        uint32_t MISO_Pin;
 
};
 
#endif
