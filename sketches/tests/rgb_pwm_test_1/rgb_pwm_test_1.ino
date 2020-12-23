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
 * RgbLED Configuration
 */
const byte RED_LED_PIN = 0;
const byte GREEN_LED_PIN = 1;
const byte BLUE_LED_PIN = 3;
const char PORT = 'B';

/**
 * Animation Configuration
 */
const byte hue_increment = 1;

////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////


HsvColor hsvColor;

RgbLED rgbLED = RgbLED(PORT, LED_TYPE_COMMON_ANODE);
//RgbLED rgbLED = RgbLED(RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN, LED_TYPE_COMMON_ANODE);



void setup() {  
  hsvColor.h = 0;
  hsvColor.s = 255;
  hsvColor.v = 64;
}



void loop() {
    hsvColor.h = hsvColor.h + hue_increment;
    
    RgbColor rgbColor = ColorConverter::HsvToRgb(hsvColor);
    
    rgbLED.setColor(rgbColor);
    rgbLED.output();
  
}
