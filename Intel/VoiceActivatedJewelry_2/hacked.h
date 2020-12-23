


// If this is defined then Serial.print(...) is disabled.
//#define ENABLE_LOW_POWER_SLEEP
#define TEST_LED_CIRCLE

/*
#include "FHT.h" // include the library for power savings.
#include "FastAnalogRead.h"

#ifdef ENABLE_LOW_POWER_SLEEP
#include "LowPower.h"
#endif
/* */


#define MIC_PIN A0
#define DIGITAL_LED_PIN 6 //11
#define POWER_LED_PIN A4
#define INTENSITY_FACTOR 15
#define NUM_PIXELS 8
/* */

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, DIGITAL_LED_PIN, NEO_GRB + NEO_KHZ800);
     
void vj_setup() {
  //Serial.begin(115200); // use the serial port
  Serial.begin(9600);
  //pinMode(DIGITAL_LED_PIN, OUTPUT);
  pinMode(MIC_PIN, INPUT);
  digitalWrite(POWER_LED_PIN, HIGH);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //EnableFastAnalogRead();  // Sample rate above voice activation.
}


void DoVis(int brightness) {
 uint8_t r, g, b;
 
 strip.setPixelColor(0, 0, 0, 0);
 for (int i = 0; i < strip.numPixels(); ++i) {
     unsigned long time_now = millis();
     double time_seconds = time_now / 1000.0;
     
     r = brightness * (sin(i*3 + time_seconds*6.0) * .5 + .5);
     //g = 32 * (sin(i + time_seconds*6.0) * .5 + .5);
     g = 0;
     b = 0; // brightness * (sin(i*5.0 + time_seconds*8.0) * .5 + .5);
  
     strip.setPixelColor(i, r, g, b);
  }
  
  strip.setPixelColor(0, 0, 0, 0);
}
/* */

void vj_loop() {
    DoVis(1);
    strip.show();
    delay(5);
}
