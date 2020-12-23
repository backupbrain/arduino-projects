// simple neopixel library!
// https://cpldcpu.wordpress.com/2014/01/14/light_ws2812-library-v2-0-part-i-understanding-the-ws2812/
// https://github.com/cpldcpu/light_ws2812/tree/master/light_ws2812_Arduino/light_WS2812

// use the cRGB struct hsv method
#define USE_HSV

#include "WS2812.h"

#define LEDCount 10
#define outputPin 7

WS2812 LED(LEDCount); 
cRGB value;

int h = 0;   //stores 0 to 614
byte steps = 1; //5; //number of hues we skip in a 360 range per update

byte sat = 255;
byte val = 127;

long sleep = 100; //delays between update
  
void setup() {
  LED.setOutput(outputPin);
}

void loop() {
  Cycle();
  
  for(int i = 0; i < LEDCount; i++)
  {
      LED.set_crgb_at(i, value);
  }
  // Sends the data to the LEDs
  LED.sync();
  
  delay(sleep);
}

void Cycle()
{
  value.SetHSV(h, sat, val);
  
  h += steps;  
  if(h > 360)
  {
      h %= 360;
  }
}
