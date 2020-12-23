/*
 * attiny85_adc.cpp
 *
 *This example shows how to use ATtiny85 Analog-Digital conversion functionality
 *
 * http://learn.warefab.com/microcontrollers/tiny-adc/
 * Created: 8/15/2014 16:47:49
 * Author: Muchiri john
 *
 *---------------------------------------
 *|     Pin | Assignment        |
 *---------------------------------------
 *| PORTB2  | Analog Sensor input   |
 ----------------------------------------
 *|     PORTB3  | digital output 1  |
 *---------------------------------------
 *| PORTB4  | digital output 2  |
 *---------------------------------------
 *
 * License - GNU General Public License.
 *
 */
 
#include <avr/io.h>
 
/*define ADC channels available on the ATtiny85*/
#define ADC0 0x00 /*channel 0*/
#define ADC1 0x01 /*channel 1*/
#define ADC2 0x02 /*channel 2*/
#define ADC3 0x03 /*channel 3*/
 
/*define adc type of bit conversion*/
#define _8bit 0
#define _10bit 1
 
/*this function initializes ADC */
static inline void init_adc()
{
    ADMUX = ADC0; //select channel 0
    //ADCSRA = 0xC3;
    ADCSRA |= ((1<<ADEN) | (1<<ADSC) | (1<<ADPS1) | (1<<ADPS0)); /*enable adc with 128 prescaler*/
    while (!(ADCSRA & (1<<ADIF))); /*wait till conversion is complete*/
    ADCSRA |= (1<<ADIF); /*clear conversion complete flag*/
}
 
/*
read adc data from the set channel
channel  - channel to read data from
bit - conversion bit length, 8 bit or 10 bit
default is 10 bit ADC conversion
*/
static inline uint16_t read_adc_data(uint8_t channel, uint8_t bit=_10bit)
{
        ADMUX = channel;
        ADCSRA |= ((1<<ADSC) | (1<<ADPS1) | (1<<ADPS0));
        while (!(ADCSRA & (1<<ADIF))); /*wait for conversion to complete*/
        ADCSRA |= (1<<ADIF); /*clear conversion complete flag bit*/
 
        if(bit) return ADC;  //return 10 bit conversion
        else return ADCH; //return 8 bit conversion
}
 
/*main function*/
int main(void)
{
    init_adc(); /*initialize adc*/
    unsigned int adc_read = 0; /*variable to hold adc read data*/
    DDRB |= _BV(PORTB3) | _BV(PORTB4);/*make PB3 and PB4 as outputs*/
 
    while(1)
    {
 
        adc_read = read_adc_data(ADC1); /*read 10 bit adc data from ADC1 channel*/
 
        if(adc_read >=0 && adc_read <=200)
        {
            PORTB |= _BV(PORTB3); /*write PB3 high*/
            PORTB &= ~_BV(PORTB4); /*write PB4 low*/
        }
        else
        {
            PORTB |= _BV(PORTB4); /*write PB4 high*/
            PORTB &= ~_BV(PORTB3); /*write PB3 low*/
        }
    }
}
