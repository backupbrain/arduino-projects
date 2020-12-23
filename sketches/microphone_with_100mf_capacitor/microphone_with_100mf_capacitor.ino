#define MIC_PIN 14
#define MIC_PIN_WITH_CAP 15

uint16_t rawSample = 0;
uint16_t capSample = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  rawSample = analogRead(MIC_PIN);
  capSample = analogRead(MIC_PIN_WITH_CAP);
  
  Serial.print(rawSample);
  Serial.print(":");
  Serial.print(capSample);
  Serial.print("\n");
  
  delay(200);
}
