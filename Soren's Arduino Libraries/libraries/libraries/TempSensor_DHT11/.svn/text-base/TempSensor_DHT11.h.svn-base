//
//    FILE: dht11.h
// VERSION: 0.4.1
// PURPOSE: DHT11 Temperature & Humidity Sensor library for Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: http://www.micro4you.com/files/sensor/DHT11.pdf
//
//     URL: http://arduino.cc/playground/Main/DHT11Lib
//
// HISTORY:
// George Hadjikyriacou - Original version
// see dht.cpp file
//

#ifndef TEMP_SENSOR_DHT11_H_
#define TEMP_SENSOR_DHT11_H_

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


class TempSensor_DHT11 {
 public:
  enum Status { DHTLIB_OK = 0, DHTLIB_ERROR_CHECKSUM = -1, DHTLIB_ERROR_TIMEOUT = -2 };

  explicit TempSensor_DHT11(int pin);

  Status Update();

  int humidity() const { return humidity_; }
  int temperature_celsius() const { return temperature_celsius_; }
  int temperature_fahrenheit() const;

  const char* version() const { return "0.4.1"; }

 private:
  int humidity_;
  int temperature_celsius_;
  int pin_;
};


#endif  // TEMP_SENSOR_DHT11_H_