#include <Servo.h>

const int flexIn = A0;
const int servoOut = 9;


int flexValue = 0;        
int servoAngle = 0;  

Servo servo1;

void setup() {
  Serial.begin(9600); 
  servo1.attach(servoOut);
}

void loop() {
  
  flexValue = analogRead(flexIn);   
  
  //servoAngle = map(flexValue, 600, 900, 0, 180);  
  servoAngle = map(flexValue, 250, 300, 0, 180);  
  
  
  
  Serial.print("sensor = " );                       
  Serial.print(flexValue);      
  Serial.print("\t output = ");      
  Serial.println(servoAngle);   
  
  servo1.write(servoAngle);
  
  //Servo::refresh();
  
}
