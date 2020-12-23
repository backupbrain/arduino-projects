/*
 * attiny85_adc_simple.cpp
 *
 * This example shows how to use ATtiny85 Analog-Digital conversion functionality
 */
 
const byte BENT_LED_PIN = 3;
const byte STRAIGHT_LED_PIN = 4;
const byte LED_PIN = 13;
const byte FLEX_SENSOR_PIN = A1;
 
unsigned int flexReading = 0;
const unsigned int FLEX_THRESHOLD = 200;
    

void setup() {
    pinMode(BENT_LED_PIN, OUTPUT);
    pinMode(STRAIGHT_LED_PIN, OUTPUT);
    pinMode(FLEX_SENSOR_PIN, INPUT);
}

void loop() {
  flexReading = analogRead(FLEX_SENSOR_PIN);
  analogWrite(LED_PIN, map(flexReading, 0, 1024, 0, 255));
 
 /*
  if(flexReading >=0 && flexReading <= FLEX_THRESHOLD) {
    digitalWrite(BENT_LED_PIN, HIGH);
    digitalWrite(STRAIGHT_LED_PIN, LOW);
  } else {
    digitalWrite(BENT_LED_PIN, LOW);
    digitalWrite(STRAIGHT_LED_PIN, HIGH);
  }
  /* */
}
 
