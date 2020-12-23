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
const char PORT = 'B';



////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

HsvColor hsvColor;

RgbLED rgbLED = RgbLED(PORT, LED_TYPE_COMMON_CATHODE);
 
 
void setup() {
  DDRB = B11111111; // set PORTD (digital 7~0) to outputs
  
  hsvColor.h = 25;
  hsvColor.s = 255;
  hsvColor.v = 25;
  
}
 
void loop() {    
    hsvColor.h = hsvColor.h;
    
    RgbColor rgbColor = ColorConverter::HsvToRgb(hsvColor);
    rgbLED.setColor(rgbColor);
    rgbLED.output();
}



