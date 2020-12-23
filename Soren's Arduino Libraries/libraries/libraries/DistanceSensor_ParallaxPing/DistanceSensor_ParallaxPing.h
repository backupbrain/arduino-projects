#ifndef __DISTANCE_SENSOR_PARALLAX_PING_H__
#define __DISTANCE_SENSOR_PARALLAX_PING_H__

#include <Arduino.h>


// Encapsulates calculating the distance using the
// the ParallaxPing distance sensor.
class DistanceSensor_ParallaxPing {
 public:
  DistanceSensor_ParallaxPing(int pin) : pin_(pin) {}
  // Returns the distance in centimeters, takes ~5ms to run.
  int PingDistanceCentimeters();
 private:
  int pin_;
};



// Returns the distnace in centimeters.
inline int DistanceSensor_ParallaxPing::PingDistanceCentimeters() {
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pin_, OUTPUT);
  digitalWrite(pin_, LOW);
  delayMicroseconds(2);
  digitalWrite(pin_, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin_, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pin_, INPUT);
  duration = pulseIn(pin_, HIGH);

  // convert the time into a distance
  cm = duration / 29 / 2;
  return cm;
}

#endif  // __DISTANCE_SENSOR_PARALLAX_PING_H__
