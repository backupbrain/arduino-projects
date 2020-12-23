// simple neopixel library!
// https://cpldcpu.wordpress.com/2014/01/14/light_ws2812-library-v2-0-part-i-understanding-the-ws2812/
// https://github.com/cpldcpu/light_ws2812/tree/master/light_ws2812_Arduino/light_WS2812


#include "WS2812.h"
#include "RainSimulator.h"

const int numPixels = 10;
const int ledPin = 7;
const int randomReferencePin = A0;

RainSimulator rainSimulator = RainSimulator(ledPin, numPixels);

long sleep = 100; //delays between update
  
void setup() {
  randomSeed(analogRead(randomReferencePin));
  Serial.begin(115200);
  Serial.println("Starting program");
}

void loop() {
  Serial.println("Updating");
  rainSimulator.render();
  delay(sleep);
}

