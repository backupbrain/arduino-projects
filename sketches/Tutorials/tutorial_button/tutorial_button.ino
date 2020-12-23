int ledPin = 13;
int buttonPin = 2;
int ledState = LOW;
int buttonState;
int lastButtonState = LOW;

long lastDebounceTime = 0;
long debounceDelay = 50;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  
  digitalWrite(ledPin, ledState);
}

void loop() {
  
  int reading = digitalRead(buttonPin);
  
  long currentTimeMillis = millis();
  
  if (reading != lastButtonState) {
    lastDebounceTime = currentTimeMillis;
  }
  
  if ((currentTimeMillis - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }
  
    digitalWrite(ledPin, ledState);
    
    lastButtonState = reading;
}
