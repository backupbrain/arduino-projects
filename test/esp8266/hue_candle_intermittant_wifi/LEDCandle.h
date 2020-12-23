/**
 * This class turns a hardware LED flickering candle on and off
 * 
 * @author Adonis Gaitatzis backupbrain@gmail.com
 * @date 2015-10-07
 */

#ifndef LEDCandle_h
#define LEDCandle_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class LEDCandle {
  public:
    LEDCandle(const byte pin);
    void setSerialNumber(String serialNumber);
    String getSerialNumber();
    void turnOn();
    void turnOff();
    boolean isOn();
  private:
    byte _pin;
    String _serialNumber;
};

#endif
