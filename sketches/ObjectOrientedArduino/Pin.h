/**
 * This class represents an Arduino CPU Pin
 * 
 * Some pins support PWM, Analog out, digital out, or default values 
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-01-28
 */
#ifndef Pin_h
#define Pin_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif


class Pin {
  
  public:
    Pin(byte pinNumber);
    ~Pin ();
    
  
  private:
    byte _pinNumber;
};

#endif 
