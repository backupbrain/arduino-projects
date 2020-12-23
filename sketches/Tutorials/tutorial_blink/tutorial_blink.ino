int led = 13;
int currentValue = LOW;
long previousMillis = 0;
long interval = 1000;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  
  if (millis() - previousMillis > interval) {
      previousMillis = millis();
      
      if (currentValue == LOW) {
        currentValue = HIGH;
      } else {
        currentValue = LOW;
      }
      
       digitalWrite(led, currentValue);
  }
  
}
