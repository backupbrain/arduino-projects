// Copyright 2012 Zach Vorhies (zach.vorhies@gmail.com)

#ifndef TEMPSENSOR_DS18S20_H_
#define TEMPSENSOR_DS18S20_H_

#include <Arduino.h>

#include <OneWire.h> // YOU MUST INCLUDE THIS IN YOUR SKETCH.

// Drives a DS18S20 temperature sensor.
// HARDWARE-LAYOUT:
//  Device has three pins: ground, signal, power (5.0v) (in that order)
//  To find the ground pin, face the flat side to your face with the pins
//  pointing downward. The left-most pin is ground, the center is signal, and the right-most
//  pin is 5v power.
// INPUT-PIN:
//  This is a digital pin. Make sure input-pin has a 4.7k pullup resister to
//  5v.
class TempSensor_DS18S20 {
 public:
  TempSensor_DS18S20(int pin);
  ~TempSensor_DS18S20();
  float TempCelsious();
  float TempFahrenheit();

 private:
  TempSensor_DS18S20(const TempSensor_DS18S20&);  // Disallow copy and assign.
  OneWire temp_sensor_;
};


/////////////////////////// Implimentation ////////////////////////////////////
inline TempSensor_DS18S20::TempSensor_DS18S20(int pin)
    : temp_sensor_(pin) {
  // Sets the input pin as a pull-up resister.
  digitalWrite(pin, HIGH);
}

inline TempSensor_DS18S20::~TempSensor_DS18S20() {
}

inline float TempSensor_DS18S20::TempCelsious() {
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if (!temp_sensor_.search(addr)) {
    //no more sensors on chain, reset search
    temp_sensor_.reset_search();
    return -1000;
  }

  if (OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if (addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }

  temp_sensor_.reset();
  temp_sensor_.select(addr);
  temp_sensor_.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = temp_sensor_.reset();
  temp_sensor_.select(addr);
  temp_sensor_.write(0xBE); // Read Scratchpad


  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = temp_sensor_.read();
  }

  temp_sensor_.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;
}

inline float TempSensor_DS18S20::TempFahrenheit() {
  return TempCelsious() * 9.f / 5.f + 32.0f;
}

#endif  // TEMPSENSOR_DS18S20_H_
