#include <stdio.h>
#include "music_svm.h"


void setup(void);
void loop(void);


/*
 procedure:
	1) read in file
	2) parse data file line by line or chunk by chunk?
	3) look at current line to see features that indicate a beat
	4) look at past X lines to see features that indicate a beat
	5) build probability table

Things to note:
	1) current time (cursor) in song
	2) assumed bpm?
	3) ignore irrelevant frequencies (> 6000 hz)
	4) weight low frequencies (<500 hz)
*/

void main(int argc, int* argv) {
	setup();
	while (true) {
		loop();
	}
}



void setup() {

}

void loop() {

}