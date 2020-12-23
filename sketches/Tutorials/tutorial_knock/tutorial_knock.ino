
const int ledPin = 9;
int potPin = A0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  
  int reading  = analogRead(potPin);
}

void loop() {

  int potreading  = analogRead(potPin);
  int brightness = map(potreading, 0, 1023, 0, 255);
  
  
  Serial.println(brightness);
  
  
  analogWrite(ledPin, brightness);
  
}
