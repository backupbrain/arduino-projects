/**
 * APA102 Test
 */


#include <stdint.h>
#include <stdio.h>

#include "APA102.h"


void setup() {
	printf("Hello World\n");


	const uint8_t numPixels = 5;
	APA102 apa102(numPixels);

	// insert several colors: red, green, blue, white, black

	RGBColor red;
	red.red = 255;
	red.green = 0;
	red.blue = 0;
	apa102.putRGBColor(red);

	RGBColor green;
	green.red = 0;
	green.green = 255;
	green.blue = 0;
	apa102.putRGBColor(green);

	RGBColor blue;
	blue.red = 0;
	blue.green = 0;
	blue.blue = 255;
	apa102.putRGBColor(blue);

	RGBColor white;
	white.red = 255;
	white.green = 255;
	white.blue = 255;
	apa102.putRGBColor(white);

	RGBColor black;
	black.red = 0;
	black.green = 0;
	black.blue = 0;
	apa102.putRGBColor(black);


	//apa102.printAPA102();
	uint32_t apa102red = apa102.RGB2APA102(red);
	uint32_t apa102green = apa102.RGB2APA102(green);
	uint32_t apa102blue = apa102.RGB2APA102(blue);
	uint32_t apa102white = apa102.RGB2APA102(white);
	uint32_t apa102black = apa102.RGB2APA102(black);
	apa102.parseOctoBytes(apa102blue);

}
void loop() {
	printf(".");
}








int main(void) {
	setup();
	/*
	while(true) {
		loop();
	}
	/* */
	return 0;
}