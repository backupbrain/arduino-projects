#include "MaxSonarRangeFinder.h"

MaxSonarRangeFinder::MaxSonarRangeFinder(int pin)
    : pin_(pin), last_pw_range_(0), last_time_ms_(0) {
  pinMode(pin_, INPUT);
}

int MaxSonarRangeFinder::Signal2Centimeters(int signal) {
  //58uS per cm
  return signal / 58;
}

// Uses Pulse-width to find the distance. Make sure the "PW" pin
// on the MaxSonar range finder.
int MaxSonarRangeFinder::PulsePWCentimeters() {
  int signal = PulsePWSignal();
  return Signal2Centimeters(signal);

}

int MaxSonarRangeFinder::PulsePWSignal() {
  long curr_time_ms = millis();
  if (last_time_ms_ + 50 > curr_time_ms) {
    //Serial.print("MaxSonarPulse is returning cached value: ");
    //Serial.println(last_pw_range_);
    return last_pw_range_;
  }
  last_time_ms_ = curr_time_ms;
  int pulse = pulseIn(pin_, HIGH);
  //Serial.print("MaxSonarPulse is :");
  //Serial.println(pulse);
  if (pulse > 0) {
    last_pw_range_ = pulse;
  }
  return last_pw_range_;
}