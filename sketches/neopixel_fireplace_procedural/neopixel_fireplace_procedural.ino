//#include "Arduino.h"
#include <Adafruit_NeoPixel.h>



/**
 * System configuration
 */

const byte POWER_LED_PIN = 13;

/**
 *  Neopixel Configuration
 */
const unsigned int LED_COUNT = 8; // number of LED pins
const byte NEOPIXEL_PIN = 3; // PWM-compatible output pin

/**
 * Animation configuration
 */
const unsigned int FRAMERATE_MS = 16; // 16 ms = 60 fps
const unsigned int TIME_INTERVAL_MIN = 500;
const unsigned int TIME_INTERVAL_MAX = 1750;


static const float H_RANGE_MIN = 0;
static const float H_RANGE_MAX = 50;
static const float S_RANGE_MIN = 0.75;
static const float S_RANGE_MAX = 1.0;
static const float V_RANGE_MIN = 0;
static const float V_RANGE_MAX = 0.3;


/**
 * Internal state
 */
 
 


typedef struct rgb {
    double r;       // percent
    double g;       // percent
    double b;       // percent
} rgb;

typedef struct hsv {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;

struct Pixel {
  hsv hsvColor;
  long timeout;
};

Pixel leds_old[LED_COUNT];
Pixel leds[LED_COUNT];

unsigned long frameStartTime_ms;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

long currentTime;


float tween(float prev, float next, float percent);
hsv randomHSV();
float randFloat(float min, float max);
int randTimeout(int min, int max);
rgb hsv2rgb(hsv in);

void setup() {
  Serial.begin(9600);
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH);
	
  frameStartTime_ms = millis();
}



void loop() {
  
  currentTime = millis();
  long startTime;
  long endTime;
  long timeDifference;
  long timeScale;
  float percentTween;
  
  
  for (unsigned int index = 0; index < strip.numPixels(); index++) {
    startTime = leds_old[index].timeout;
    endTime = leds[index].timeout;
    timeDifference = currentTime - endTime;
    
    if (timeDifference < 0) {
      timeScale = leds[index].timeout - leds_old[index].timeout;
      if (timeScale <= 0) {
        percentTween = 1;
      } else {
        percentTween = 0 - timeDifference/(leds[index].timeout - leds_old[index].timeout);
      }
      
      hsv hsvColor;
      hsvColor.h = tween(leds_old[index].hsvColor.h, leds[index].hsvColor.h, percentTween);
      hsvColor.s = tween(leds_old[index].hsvColor.s, leds[index].hsvColor.s, percentTween);
      hsvColor.v = tween(leds_old[index].hsvColor.v, leds[index].hsvColor.v, percentTween);
      
      if (index == 0) {
        Serial.print("{ ");
        Serial.print(hsvColor.h);
        Serial.print(", ");
        Serial.print(hsvColor.s);
        Serial.print(", ");
        Serial.print(hsvColor.v);
        Serial.println(" }");
      }
      
      rgb rgbColor = hsv2rgb(hsvColor);
      
      strip.setPixelColor(index, strip.Color(rgbColor.r*255, rgbColor.g*255, rgbColor.b*255));
    } else {
      leds_old[index].hsvColor.h = leds[index].hsvColor.h;
      leds_old[index].hsvColor.s = leds[index].hsvColor.s;
      leds_old[index].hsvColor.v = leds[index].hsvColor.v;
      leds_old[index].timeout = leds[index].timeout;
      
      hsv newHSV = randomHSV();
      
      leds[index].hsvColor.h = newHSV.h;
      leds[index].hsvColor.s = newHSV.s;
      leds[index].hsvColor.v = newHSV.v;
      leds[index].timeout = currentTime + randTimeout(TIME_INTERVAL_MIN, TIME_INTERVAL_MAX);
    }
  }
   strip.show();  
}


float tween(float prev, float next, float percent) {
  float result = (prev-next)*percent + next;
  return result;
}
hsv randomHSV() {
  hsv result;
  result.h = randFloat(H_RANGE_MIN, H_RANGE_MAX);
  result.s = randFloat(S_RANGE_MIN, S_RANGE_MAX);
  result.v = randFloat(V_RANGE_MIN, V_RANGE_MAX);
  result.v = result.v*result.v*(1/(V_RANGE_MAX-V_RANGE_MIN));
  
  return result;
}
float randFloat(float min, float max) {
  return (rand()/(max-min)) + min;
}
int randTimeout(int min, int max) {
  return (rand()%(max-min)) + min;
}

rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;     
}


