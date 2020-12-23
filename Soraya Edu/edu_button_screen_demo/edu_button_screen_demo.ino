#include "PulseTimer.h"
#include "buttonReader.h"


const int buttonDebounceTimeout_ms = 20;
PulseTimer debounceTimer(buttonDebounceTimeout_ms);


static int state = 0; // state determines which image to display
static int maxState = 4;


#include "einkScreen.h"

void setup() {
  buttonReader_setup();
  einkScreen_setup();
  einkScreen_drawScreen();
}



void loop() {
  if (!debounceTimer.update()) { return; }

  if (buttonReader_buttonClicked()) {
    // button clicked
    state++;
    if (state > maxState) {
      state = 0;
    }
    einkScreen_drawScreen();
    Serial.println("Button Clicked");
  }
}

