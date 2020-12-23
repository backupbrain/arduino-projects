const unsigned int ledPin = 13;
const unsigned int blinkTimeout_ms = 200;
const unsigned int baudRate = 9600;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);

  
  pinMode(ledPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);
  delay(blinkTimeout_ms);
  digitalWrite(ledPin, LOW);
  delay(blinkTimeout_ms);
  Serial.println("Hello");

}
