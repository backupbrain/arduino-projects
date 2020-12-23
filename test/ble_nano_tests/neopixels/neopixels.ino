/* Copyright (c) 2015 bickster, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <Arduino.h>
#include "mbed.h"

/*
#include "NrfNeopixel.h"

const uint8_t pin = P0_8;
const uint32_t numPixels = 8;
NrfNeopixel strip = NrfNeopixel(pin, numPixels);

uint8_t led = 0;

const unsigned int ledPin = 13;
const unsigned long blinkTimeout = 1000;



void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World");

  strip.init();
}
void loop() {
    // To show more than one led call this funtion n number of times.
    strip.setPixelColor(led, 128, 64, 32);
    strip.show();

    led++;
    if (led > numPixels) {
      led = 0;
    }
    
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);
  delay(blinkTimeout);
  digitalWrite(ledPin, LOW);
  delay(blinkTimeout);
    
}

/* */

#include "nrf_neopixel.h"

neopixel_strip_t m_strip;
uint8_t dig_pin_num = P0_8;
uint8_t leds_per_strip = 8;
uint8_t errorhappened;
uint8_t led_to_enable = 1;
uint8_t red = 255;
uint8_t green = 0;
uint8_t blue = 159;


const unsigned int ledPin = 13;
const unsigned long blinkTimeout = 1000;

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World");
  
  neopixel_init(&m_strip, dig_pin_num, leds_per_strip);
  neopixel_clear(&m_strip);
}
void loop() {
    // To show more than one led call this funtion n number of times.
    errorhappened = neopixel_set_color_and_show(&m_strip, led_to_enable, red, green, blue);

    
    led_to_enable++;
    if (led_to_enable > leds_per_strip) {
      led_to_enable = 0;
    }
    
    if (errorhappened) {
        //led_to_enable was not within number leds_per_strip
    }
    //delay(200);

    
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);
  delay(blinkTimeout);
  digitalWrite(ledPin, LOW);
  delay(blinkTimeout);
    
}
/* */




