// Copyright 2012 Zach Vorhies (zach.vorhies@gmail.com)

#include <Arduino.h>

#ifndef RANGEFINDER_SHARP_IR_H_
#define RANGEFINDER_SHARP_IR_H_


class RangeFinderSharpIR {
 public:
  RangeFinderSharpIR(int analogue_pin) : analogue_pin_(analogue_pin) {
    // Setup the pin so that it is a pulldown.
    pinMode(analogue_pin_, INPUT);
    digitalWrite(analogue_pin_, LOW);
    // Good tradeoff for accuracy vs latency of sensor.
    set_multi_sample_count(16);
  }
  float SampleDistanceMeters() {
    float volts = MultiSampleVolts(multi_sample_count_);

    float distance_cm = 65*pow(volts, -1.10);          // worked out from graph 65 = theretical distance / (1/Volts)S - luckylarry.co.uk
    float distance_meters = distance_cm / 100.0f;
    // Serial.print(distance_meters);                       // print the distance
    return distance_meters;
  }

  float SampleDistanceInches() {
    return 39.3701f * SampleDistanceMeters();
  }

  void set_multi_sample_count(int val) {
    multi_sample_count_ = constrain(val, 0, 4096);
  }

 private:
  float MultiSampleVolts(int count) {
    float avg = SampleVolts();
    for (int i = 1; i < count; ++i) {
      avg += SampleVolts();
      delayMicroseconds(32);  // Lightly tested.
    }
    avg /= static_cast<float>(count);
    return avg;
  }

  float SampleVolts() {
    //analogRead(analogue_pin_);  // first one is throw away.
    //delay(1);
    float volts = analogRead(analogue_pin_)*0.0048828125;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
    return volts;
  }

  int analogue_pin_;
  int multi_sample_count_;
};

#endif // RANGEFINDER_SHARP_IR_H_
