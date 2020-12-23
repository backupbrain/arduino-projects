/**
 * This class represents an Arduino CPU Pin
 * 
 * Some pins support PWM, Analog out, digital out, or default values 
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-01-28
 */

#include "Pin.h"

Pin::Pin(byte pinNumber): _pinNumber(pinNumber) {
  
}
Pin::~Pin() {
  
}

