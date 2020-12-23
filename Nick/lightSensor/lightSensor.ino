

// Which pin is the onboard LED connected to?
int lightSensorPin = A0;

// Which pin is the onboard LED connected to?
int ledPin = 11;

void setup() {
  // initialize a connection to the computer
  Serial.begin(9600);

  Serial.println("Starting Program");
}

void loop() {
  // read the value from the light sensor
  int LightSensorValue = analogRead(lightSensorPin);
  
  int scaledLightSensorValue = map(LightSensorValue, 40, 180, 255, 0);

  // Change the LED brightness relative to LightSensorValue
  analogWrite(ledPin, scaledLightSensorValue);

  // Print data to the computer screen
  Serial.println(LightSensorValue);}


