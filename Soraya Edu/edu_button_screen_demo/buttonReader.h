const int buttonPin = 0;
int lastButtonValue = LOW;


void buttonReader_setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  lastButtonValue = digitalRead(buttonPin);
}

bool buttonReader_buttonClicked() {
  int currentButtonValue = digitalRead(buttonPin);
  if (currentButtonValue != lastButtonValue) {
    lastButtonValue = currentButtonValue;
    return true;
  }
  return false;
}
