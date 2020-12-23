const int analogInPin = A0;
const int ledOutPin = 13;

const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

int sensorValue = 0;
int outputValue = 0;

void setup() { 
  pinMode(ledOutPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  Serial.begin(9600);
}

void loop() { 
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  //analogWrite(ledOutPin, outputValue);
  
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);
  
  
 setColor(outputValue, outputValue, outputValue);
  
  if (sensorValue > 900) {
    digitalWrite(ledOutPin, HIGH);
    
  } else {
    digitalWrite(ledOutPin, LOW);
  }
  
  delay(500);
  
  
  
  
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
