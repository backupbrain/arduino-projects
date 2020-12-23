/**
 * This class turns a hardware LED flickering candle on and off
 * 
 * @author Adonis Gaitatzis backupbrain@gmail.com
 * @date 2015-10-07
 */

#include "LEDCandle.h"

LEDCandle::LEDCandle(const byte pin) {
  this->_pin = pin;
  pinMode(this->_pin, OUTPUT);
}

void LEDCandle::setSerialNumber(String serialNumber) {
  this->_serialNumber = serialNumber;
}
String LEDCandle::getSerialNumber() {
  return this->_serialNumber;
}
void LEDCandle::turnOn() {
  digitalWrite(this->_pin, HIGH);
}

void LEDCandle::turnOff() {
  digitalWrite(this->_pin, LOW);
}

