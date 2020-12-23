/**
 * APA102 Test
 */


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
 
#include "APA102FrameManager.h"


void setup() {
	printf("Hello World\n");


	const uint8_t numPixels = 5;
	APA102FrameManager frameManager(numPixels);

	// insert several colors: red, green, blue, white, black

	RGBColor red;
	red.red = 255;
	red.green = 0;
	red.blue = 0;

	RGBColor green;
	green.red = 0;
	green.green = 255;
	green.blue = 0;

	RGBColor blue;
	blue.red = 0;
	blue.green = 0;
	blue.blue = 255;

	RGBColor white;
	white.red = 255;
	white.green = 255;
	white.blue = 255;

	RGBColor black;
	black.red = 0;
	black.green = 0;
	black.blue = 0;

	/*
	//apa102.printAPA102();
	uint32_t apa102red = apa102.RGB2APA102(red);
	uint32_t apa102green = apa102.RGB2APA102(green);
	uint32_t apa102blue = apa102.RGB2APA102(blue);
	uint32_t apa102white = apa102.RGB2APA102(white);
	uint32_t apa102black = apa102.RGB2APA102(black);
	apa102.parseOctoBytes(apa102blue);
	/* */

	uint8_t numFrames = 3;

	uint32_t frameStart = 0x00;
	uint32_t frameEnd = 0xFFFFFFFF;

	printf("Inserting Data\n");
	printf("--------------\n");
	for (int frame = 0; frame<numFrames; frame++) {
		printf("Frame %d\n", frame);

		frameManager.parseOctoBytes(frameStart);
		RGBColor::printRGB(&red); printf("\t"); RGBColor::printAPA102(RGBColor::RGB2APA102(&red)); printf("\n");
		frameManager.parseOctoBytes(RGBColor::RGB2APA102(&red));
		RGBColor::printRGB(&green); printf("\t"); RGBColor::printAPA102(RGBColor::RGB2APA102(&green)); printf("\n");
		frameManager.parseOctoBytes(RGBColor::RGB2APA102(&green));
		RGBColor::printRGB(&blue); printf("\t"); RGBColor::printAPA102(RGBColor::RGB2APA102(&blue)); printf("\n");
		frameManager.parseOctoBytes(RGBColor::RGB2APA102(&blue));
		RGBColor::printRGB(&red); printf("\t"); RGBColor::printAPA102(RGBColor::RGB2APA102(&red)); printf("\n");
		frameManager.parseOctoBytes(RGBColor::RGB2APA102(&white));
		RGBColor::printRGB(&black); printf("\t"); RGBColor::printAPA102(RGBColor::RGB2APA102(&black)); printf("\n");
		frameManager.parseOctoBytes(RGBColor::RGB2APA102(&black));
		frameManager.parseOctoBytes(frameEnd);
		printf("------------------\n");
	}

	// read each frame
	printf("\n\n");
	printf("Retrieving Data\n");
	printf("---------------\n");
	for (int frameIndex = 0; frameIndex < numFrames; frameIndex++) {
		APA102Frame* frame = frameManager.getFrame(frameIndex);
		fprintf(stdout, "Frame %d: \n", frameIndex);
		for (int pixel = 0; pixel < numPixels; pixel++) {
			uint32_t apa102 = frame->getPixel(pixel);
			RGBColor color = RGBColor::APA1022RGB(apa102);
			RGBColor::printRGB(&color);
			printf("\t");
			RGBColor::printAPA102(apa102);
			printf("\n");
		}
		printf("------------------\n");
	}




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