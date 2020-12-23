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
 
#include "spi_master.h"
 
/**********************************************************************
name :
function : 
**********************************************************************/
SPIClass::SPIClass(NRF_SPI_Type *_spi) : spi(_spi)
{
    //do nothing
}
 
/**********************************************************************
name :
function : 
**********************************************************************/
void SPIClass::setCPOL( bool active_low)
{
    if(active_low)
    {
        spi->CONFIG |= (SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos);
    }
    else
    {
        spi->CONFIG |= (SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos);
    }
}
 
/**********************************************************************
name :
function : 
**********************************************************************/
void SPIClass::setCPHA( bool trailing)
{
    if(trailing)
    {
        spi->CONFIG |= (SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos);
    }
    else
    {
        spi->CONFIG |= (SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos);
    }
 
}
 
/**********************************************************************
name :
function : MSBFIRST, LSBFIRST
**********************************************************************/
void SPIClass::setBitORDER( bitorder  bit)
{
    if(bit == msbfirst)
    {
        spi->CONFIG |= (SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos);
    }
    else
    {
        spi->CONFIG |= (SPI_CONFIG_ORDER_LsbFirst << SPI_CONFIG_ORDER_Pos);
    }
}
 
/**********************************************************************
name :
function : 
**********************************************************************/
void SPIClass::setFrequency(uint8_t speed)
{
    if (speed == 0)
    {
        spi->FREQUENCY = SPI_FREQUENCY_125K;
    }
    else if (speed == 1)
    {
        spi->FREQUENCY = SPI_FREQUENCY_250K;
    }
    else if (speed == 2)
    {
        spi->FREQUENCY = SPI_FREQUENCY_500K;
    }
    else if (speed == 3)
    {
        spi->FREQUENCY = SPI_FREQUENCY_1M;
    }
    else if (speed == 4)
    {
        spi->FREQUENCY = SPI_FREQUENCY_2M;
    }
    else if (speed == 5)
    {
        spi->FREQUENCY = SPI_FREQUENCY_4M;
    }
    else if (speed == 6)
    {
        spi->FREQUENCY = SPI_FREQUENCY_8M;
    }
    else
    {
        spi->FREQUENCY = SPI_FREQUENCY_4M;
    }   
}
 
/**********************************************************************
name :
function : 
**********************************************************************/
void SPIClass::setSPIMode( uint8_t mode )
{
    if (SPI_MODE0 == mode)
    {
        setCPOL(0);
        setCPHA(0);     
    }
    else if (mode == SPI_MODE1)
    {
        setCPOL(0);
        setCPHA(1);
    }
    else if (mode == SPI_MODE2)
    {
        setCPOL(1);
        setCPHA(0);
    }
    else if (mode == SPI_MODE3)
    {
        setCPOL(1);
        setCPHA(1);
    }
    else
    {
        setCPOL(0);
        setCPHA(0);         
    }
}
 
/**********************************************************************
name :
function : 
**********************************************************************/
void SPIClass::begin(uint32_t sck, uint32_t mosi, uint32_t miso)
{
 
        SCK_Pin = sck;
        MOSI_Pin = mosi;
        MISO_Pin = miso;
        
        NRF_GPIO->PIN_CNF[SCK_Pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
                                        | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                                        | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                                        | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                                        | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);    
                                        
        NRF_GPIO->PIN_CNF[MOSI_Pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
                                        | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                                        | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                                        | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                                        | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
                                        
        NRF_GPIO->PIN_CNF[MISO_Pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
                                        | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                                        | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                                        | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                                        | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);                                     
        spi->PSELSCK  = SCK_Pin;
        spi->PSELMOSI = MOSI_Pin;
        spi->PSELMISO = MISO_Pin;       
        
        setFrequency(SPI_2M);
        setSPIMode(SPI_MODE0);
        setBitORDER(msbfirst);
        
        spi->EVENTS_READY = 0;
        spi->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
    
}
 
/**********************************************************************
name :
function : 
**********************************************************************/
void SPIClass::begin()
{
    begin(SCK, MOSI, MISO);
}
 
/**********************************************************************
name :
function : 
**********************************************************************/
uint8_t SPIClass::transfer(uint8_t data)
{
    while( spi->EVENTS_READY != 0U );
    
    spi->TXD = (uint32_t)data;
    
    while(spi->EVENTS_READY == 0);
    
    data = (uint8_t)spi->RXD;
    
    spi->EVENTS_READY = 0;
    
    return data;
}
 
/**********************************************************************
name :
function : 
**********************************************************************/
void SPIClass::endTransfer()
{
    spi->ENABLE = (SPI_ENABLE_ENABLE_Disabled << SPI_ENABLE_ENABLE_Pos);
}
 
/**********************************************************************
name :
function : 
**********************************************************************/
 
//SPIClass SPI(NRF_SPI1);
