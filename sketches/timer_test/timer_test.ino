#include "TimerOne.h"
     
     
Timer t;
int pin = 13;
     
     
void setup() {
  pinMOde(pin, OUTPUT);
  Timer1.initialize(500000);
  Timer1.pwm(9, 512);
  Timer1.attachInterrupt(callback);
}
     
     
    void loop()
    {
      t.update();
    }
     
     
    void takeReading()
    {
      Serial.println(analogRead(0));
    }

