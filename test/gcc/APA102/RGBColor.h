/**
 * RGB Color
 * 
 * This structure represents RGB Color values
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-28
 */


#ifndef RGBColor_h
#define RGBColor_h


#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#include <stdio.h>    /* fprintf */
#endif

class RGBColor {
	public:
		static uint32_t RGB2APA102(RGBColor* color);
		static RGBColor  APA1022RGB(uint32_t apa102);

		static void printRGB(RGBColor* color);
		static void printAPA102(uint32_t apa102);
  	uint8_t 
  		red,
  		green,
  		blue;
};

#endif