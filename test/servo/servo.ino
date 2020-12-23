const byte ledPin = 3;
const byte lightSensorPin = A0;

const unsigned int sensorMin = 50;
const unsigned int sensorMax = 600;


byte ledBrightness = 0;
unsigned int lightSensorValue;


void setup() {
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
}

void loop() {
  lightSensorValue = min(sensorMax,analogRead(lightSensorPin));
  lightSensorValue = max(sensorMin,lightSensorValue);
  
  ledBrightness = 255 - map(lightSensorValue, sensorMin, sensorMax, 0, 254); 
  
  Serial.println(ledBrightness);
  
  analogWrite(ledPin, ledBrightness);

  delay(50);
} 
