#ifndef __LED_H__
#define __LED_H__

#include <Arduino.h>

// Controls an LED light connect to the specified pin.
class Led {
 public:
  Led(int led_pin) : led_pin_(led_pin), curr_brightness_(0) {
    pinMode(led_pin_, OUTPUT);
    digitalWrite(led_pin_, LOW);
  }

  void Increment() { StepLed(1, led_pin_, &curr_brightness_); }
  void Decrement() { StepLed(-1, led_pin_, &curr_brightness_); }
  // Returns true if the led has transitioned to that brightness.
  bool TransitionBrightness(int brightness, int transition_speed);
  void Off() { Set(0); }
  void On() { Set(255); }
  void Set(int value_brightness);
  int Get() const { return curr_brightness_; }

 private:
  static int Clamp(int val, int min_val, int max_val);
  static int MapLedBrightness(int val);
  static void StepLed(int amount, int led, int* led_val);

  int led_pin_;
  int curr_brightness_;
};

class TimedLed : public Led {
 public:
  TimedLed(int pin) : Led(pin), rate_(0.0f), power_(0.0f), target_power_(0.0) {}
  void Update(float dt);
  void Transition(float target_power, float transition_time_seconds);
  float rate_;
  float power_;
  float target_power_;
};

#endif  // __LED_H__
