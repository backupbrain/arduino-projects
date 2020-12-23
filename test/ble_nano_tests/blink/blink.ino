/**
 * Blink sketch on BLE Nano (nrf51:822)
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-16
 */
const unsigned int ledPin = 13;
const unsigned long blinkTimeout = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);
  delay(blinkTimeout);
  digitalWrite(ledPin, LOW);
  delay(blinkTimeout);
  
  Serial.println("Blink!");

}
