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
 * FlexSensor Sampler Configuration
 */
const byte FLEX_SENSOR_PIN = PB2;

/**
 * RgbLED Configuration
 */
const char PORT = 'B';


/**
 * Animation Configuration
 */
const byte hue_increment = 1;


////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

unsigned int flexReading = 0;
unsigned long lastAnimationFrameUpdateTime;

HsvColor hsvColor;

RgbLED rgbLED = RgbLED(PORT, LED_TYPE_COMMON_ANODE);
 
 
void setup() {
  DDRB = B11111111; // set PORTD (digital 7~0) to outputs
  
  hsvColor.h = 0;
  hsvColor.s = 255;
  hsvColor.v = 255;
  
  pinMode(FLEX_SENSOR_PIN, INPUT);
}
 
void loop() {
    flexReading = analogRead(FLEX_SENSOR_PIN);
    
    hsvColor.h = hsvColor.h + hue_increment;
    
    RgbColor rgbColor = ColorConverter::HsvToRgb(hsvColor);
    rgbLED.setColor(rgbColor);
    rgbLED.output();
}



