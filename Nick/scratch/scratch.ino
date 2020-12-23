

// Which pin is the onboard LED connected to?
int onBoardLedPin = 13;



void setup() {
  // initialize a connection to the computer
  Serial.begin(9600);

  Serial.println("Starting Program");
}

void loop() {
  // turn the led on by switching the pin into "HIGH" mode
  digitalWrite(onBoardLedPin, HIGH);

  // Print data to the computer screen
  Serial.println("Led Turned on");

  // wait for 1/2 a second (500 milliseconds)
  delay(500);
// turn the led on by switching the pin into "LOW" mode
  digitalWrite(onBoardLedPin, LOW);
  
  // Print data to the computer screen
  Serial.println("Led Turned off");
  
  // wait for 1/2 a second (500 milliseconds)
  delay(500);}


