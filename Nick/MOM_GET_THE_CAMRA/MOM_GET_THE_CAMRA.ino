#include <Adafruit_NeoPixel.h>
const byte neopixelPin = 6;
const byte micPin = A5;

const uint8_t numPixels = 12;

const uint8_t numMicReadings = 255;
const uint8_t micReadDelay_ms = 200;

const uint8_t micRangeCutoff = 20;

uint8_t colors[numPixels] = { 0 };

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numPixels, neopixelPin, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);

  pixels.begin(); // This initializes the NeoPixel library.
  for (int i=0; i<numPixels; i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
  pixels.show();
}

void loop() {
  int micMin = 255;
  int micMax = 0;
  for (int i=0; i< numMicReadings; i++) {
    int micValue = analogRead(micPin);
    micMax = max(micMax, micValue);
    micMin = min(micMin, micValue);
  }

  // calculate range
  int range = micMax - micMin;
  range = constrain(range, micRangeCutoff, 1023);
  
  int brightness = map(range, micRangeCutoff, 1023, 0, 100);

  Serial.println(brightness);
  
  for (int i=0; i< numPixels-1; i++) {
    colors[i] = colors[i+1];
    pixels.setPixelColor(i, pixels.Color(colors[i],colors[i],colors[i]));
  }
  colors[numPixels-1] = brightness;
  
  pixels.setPixelColor(numPixels-1, pixels.Color(brightness, brightness, brightness));
  
  pixels.show(); // This sends the updated pixel color to the hardware.
}
