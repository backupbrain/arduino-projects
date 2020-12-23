#include <FastLED.h>
#define NUM_LEDS 12
#define DATA_PIN 7

const int randomReferencePin = A0;

CRGB leds[NUM_LEDS];

uint8_t LED_MAX_BRIGHTNESS = 94;
uint8_t LED_MIN_BRIGHTNESS = 35;

uint8_t LED_STATE_OFF = 0x00;
uint8_t LED_STATE_RISING = 0x01;
uint8_t LED_STATE_FALLING = 0x02;
uint8_t ledStates[NUM_LEDS] = { LED_STATE_OFF };


void setup() {
  randomSeed(analogRead(randomReferencePin));
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

}

const CRGB TEST_COLOR = CRGB::Black;
int rainProbability = 5; // 20%;
void loop() {


  for (int i=NUM_LEDS-1; i>=1; i--) {
    if (leds[i-1] != TEST_COLOR) {
      leds[i-1] = CRGB::Black;
      leds[i] = CHSV(209, 100, 94);
    }
  }

  
  int rain = random(0,100);
  int brightness = random(LED_MIN_BRIGHTNESS, LED_MAX_BRIGHTNESS);
  if (rain < rainProbability) { // new raindrop
      leds[0] = CHSV(209, 100, brightness);
  }
  
  FastLED.show();

  /*  
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
      leds[dot] = CHSV(296, 100, 94);
      FastLED.show();
      // clear this led for the next time around the loop
      leds[dot] = CRGB::Black;
      delay(30);
  }
  */
  delay(50);


  
  //rainSimulator.render();
  //delay(sleep);

}





