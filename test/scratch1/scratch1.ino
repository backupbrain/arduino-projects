void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);

  Serial.print("CPU Speed: ");
  Serial.print(F_CPU);
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:

}
