int minPin = 0;
int maxPin = 44;

const unsigned int micPin = 30;  // ADC7
const unsigned int rightPhotoSensorPin = 37; // ADC0
void setup() {
  Serial.begin(9600);
  digitalWrite(micPin, INPUT);
  digitalWrite(micPin, INPUT);
  for (int index=minPin; index<maxPin; index++) {
    pinMode(index, OUTPUT);
    digitalWrite(index, HIGH);
  }
  for (int index=minPin; index<maxPin; index++) {
    pinMode(index, INPUT);
  }
}

void loop() {
  int value = 0;
  for (int index=minPin; index<maxPin; index++) {
    value =analogRead(index);
    Serial.print(value);
    Serial.print(" ");
  }
  Serial.println();
}
