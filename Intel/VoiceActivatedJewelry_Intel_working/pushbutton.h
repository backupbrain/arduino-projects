#include <Arduino.h>



void pushbutton_setup(const uint8_t buttonPin) {
  pinMode(buttonPin, INPUT);

}

bool readPushButton(const uint8_t buttonPin) {
  // read the state of the switch into a local variable:

  bool retval = true;

  /*  
  pinMode(buttonPin, OUTPUT);
  delay(1);
  digitalWrite(buttonPin, LOW);
  delay(1);
  pinMode(buttonPin, INPUT);
  delay(1);
  int buttonState = digitalRead(buttonPin);
  */

  pinMode(buttonPin, INPUT);
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == 1) {
    retval = false;
  }
  
  return retval;
}



