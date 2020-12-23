/**
 * Sound Reactive Pillow - reacts to sound by showing volume as light intensity,
 * 
 * @author Tony Gaitatzis 2015
 */
#include "ColorConverter.h"
#include "RgbLED.h"


////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE CONSTANTS
// Change these to change how the program records and processes data
////////////////////////////////////////////////////////////////////////////////

/** 
 * System Configuration
 */
const byte POWER_LED_PIN = 1;
const boolean POWER_LED_ON = false;

/**
 * FlexSensor Sampler Configuration
 */
const byte FLEX_SENSOR_PIN = A1;

/**
 * RgbLED Configuration
 */
const char PORT = 'B';


/**
 * Animation Configuration
 */
const byte hue_increment = 1;
const unsigned int animationFrameTimeout_ms = 100;
unsigned int minFlexValue = 100;
unsigned int maxFlexValue = 100;
byte maxSaturation = 255;
byte minSaturation = 0;




////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

unsigned int flexReading = 0;

unsigned int lastAnimationFrameUpdateTime = 0;

HsvColor hsvColor;

RgbLED rgbLED = RgbLED(PORT, LED_TYPE_COMMON_ANODE);



void setup() {
  pinMode(FLEX_SENSOR_PIN, INPUT);
  
  // initialize the flex sensor values
  flexReading = analogRead(FLEX_SENSOR_PIN);
  maxFlexValue = flexReading;
  minFlexValue = flexReading;
  
  
  hsvColor.h = 100;
  hsvColor.s = 254;
  hsvColor.v = 255;
  
}

void loop() {
  unsigned long currentTime = millis();
 
    flexReading = analogRead(FLEX_SENSOR_PIN);
    if (flexReading > maxFlexValue) maxFlexValue = flexReading;
    if (flexReading < minFlexValue) minFlexValue = flexReading;
    
    hsvColor.s = map(flexReading,  minFlexValue, maxFlexValue, maxSaturation, minSaturation);
    
    hsvColor.h = hsvColor.h + hue_increment;
    
  
    RgbColor rgbColor = ColorConverter::HsvToRgb(hsvColor);
    
    
    rgbLED.setColor(rgbColor);
    rgbLED.output();
    
    lastAnimationFrameUpdateTime = currentTime;
  
}




