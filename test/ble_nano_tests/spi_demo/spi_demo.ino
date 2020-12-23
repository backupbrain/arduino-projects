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
 
#include "mbed.h"
#include "spi_master.h"
 
DigitalOut spi_cs(P0_7);
 
SPIClass SPI1(NRF_SPI1);
 
//Serial pc(USBTX, USBRX);
 
void Flash_Buff_WriteBytes(uint16_t addr, uint8_t *pbuf, uint16_t len)
{
    uint16_t index;
    
    spi_cs = 0;
    wait_us(200);
    
    SPI1.transfer(0x84);   
    SPI1.transfer(0x00);    
    SPI1.transfer((uint8_t)(addr>>8));    
    SPI1.transfer((uint8_t)addr);     
    for(index=0; index<len; index++)
    {
        SPI1.transfer(*pbuf);
        pbuf++;        
    }
    
    wait_us(200);
    spi_cs = 1;
}
 
void Flash_Buff_ReadBytes(uint16_t addr, uint8_t *pbuf, uint16_t len)
{
    uint16_t index;
    
    spi_cs = 0;
    wait_us(200);
    
    SPI1.transfer(0xD1);   
    SPI1.transfer(0x00);    
    SPI1.transfer((uint8_t)(addr>>8));    
    SPI1.transfer((uint8_t)addr);     
    for(index=0; index<len; index++)
    {
        *pbuf = SPI1.transfer(0x00);
        pbuf++;        
    }
    
    wait_us(200);
    spi_cs = 1;
}
 
uint8_t i;
uint8_t wt_buf[7] = {'H','e','l','l', 'o', '\r', '\n'};
uint8_t rd_buf[7];
 
int main(void)
{   
    Serial.begin(9600);
    delay(8);
    spi_cs = 1;
    Serial.print("SPI Demo Start \r\n");
    
    SPI1.begin();
    //SPI1.begin(P0_8, P0_11, P0_09);//SCK, MOSI, MOSI
    
    delay(1);
    Flash_Buff_WriteBytes(0, wt_buf, 7);
    while(1) {
         memset(rd_buf, 0x00, 7);
         Flash_Buff_ReadBytes(0, rd_buf, 7);
         for(i=0; i<7; i++)
            Serial.print(rd_buf[i]);
         wait(1);
    }
}
 
 
 
 
 
 
 
