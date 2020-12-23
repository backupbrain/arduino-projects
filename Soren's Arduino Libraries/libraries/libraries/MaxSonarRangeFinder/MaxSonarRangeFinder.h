#ifndef __MAXSONARRANGEFINDER_H__
#define __MAXSONARRANGEFINDER_H__

#include <Arduino.h>

// Controls a MaxSonar EZ-series range finders.
class MaxSonarRangeFinder {
 public:
  MaxSonarRangeFinder(int pin);
  // Uses Pulse-width to find the distance. Make sure the "PW" pin
  // on the MaxSonar range finder.
  int PulsePWCentimeters();
  int PulsePWSignal();

  static int Signal2Centimeters(int signal);
 private:
  int pin_;
  // Sometimes pulseIn(pin_, HIGH) (PW method) returns negative values.
  // This value holds the previous return value for PingRangeCentimetersUsingPW().
  int last_pw_range_;
  long last_time_ms_;
};

#endif  // __MAXSONARRANGEFINDER_H__
