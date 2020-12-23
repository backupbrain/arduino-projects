int MIC_PIN = A0;
int lastValue = 0;
int maxValue = 0;
int minValue = 1024;

void setup() {
  Serial.begin(9600);
}

 
void loop() {
  
  
  int value = analogRead(MIC_PIN); // get adc value
  
  if (value > maxValue) maxValue = value;
  if (value < minValue) minValue = value;
  
  Serial.print(minValue);
  Serial.print(":");
  Serial.print(maxValue);
  Serial.println();
}
