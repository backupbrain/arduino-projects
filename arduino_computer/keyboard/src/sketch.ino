#include "Keyboard.h"

// for compatibility, we should include these function headers
void setup(void);
void loop(void);


Keyboard keyboard = Keyboard();

// let's put this in for portability!
void main(void) {
	setup();
	while(true) {
		loop();
	}
}


void setup() {
	Serial.begin(SERIAL_BAUDRATE);
}

void loop() {
	char asciiChar = 0x00;
	// check if the key codes have been activated
	// by checking for key downs
	bool keyboardStateChanged = Keyboard.pollState();

	if (keyboardStateChanged) {
		asciiChar = Keyboard.getCharPressed();

		if (asciiChar > 0) {
			Serial.write(asciiChar);
		}
	}
}
