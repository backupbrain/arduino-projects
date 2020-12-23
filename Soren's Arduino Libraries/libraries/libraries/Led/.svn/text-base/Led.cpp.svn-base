
#include "Led.h"

bool Led::TransitionBrightness(int brightness, int transition_speed) {
  if (curr_brightness_ < brightness) {
    Set(min(curr_brightness_ + transition_speed, brightness));
  } else {
    Set(max(curr_brightness_ - transition_speed, brightness));
  }
  return brightness == curr_brightness_;
}

void Led::Set(int value_brightness) {
  int val = Clamp(value_brightness, 0, 255);
  if (curr_brightness_ == val) {
    return;
  }
  curr_brightness_ = val;
  if (0 == curr_brightness_) {
   digitalWrite(led_pin_, LOW);
 } else {
   analogWrite(led_pin_, curr_brightness_);
 }
}

int Led::Clamp(int val, int min_val, int max_val) {
  if (val < min_val) {
    return min_val;
  }
  if (val > max_val) {
    return max_val;
  }
  return val;
}

void Led::StepLed(int amount, int led, int* led_val) {
  *led_val = Clamp(*led_val + amount, 0, 255);

  int led_final_val = MapLedBrightness(*led_val);

  analogWrite(led, led_final_val);
}

int Led::MapLedBrightness(int val) {
  float f = static_cast<float>(val) / 255.0f;
  // f = f * f * f * f;  // f^4, seems to give a better fade in.
  // f = sin(f * 3.124 / 2.0);
  //f = sqrt(f);
  int rtn = static_cast<int>(f * 255.0);
  return rtn;
}

void TimedLed::Update(float dt) {
  // If nothing to do then return.
  if (power_ == target_power_) {
    return;
  }
  // move the power toward target_power_, in position of dt.
  if (power_ < target_power_) {
    power_ = min(power_ + (rate_ * dt), target_power_);
  } else {
    power_ = max(power_ - (rate_ * dt), target_power_);
  }
  // Then finally converrt the power to the led integer mapping.
  Led::Set(static_cast<int>(power_ * 255.0f));
}

void TimedLed::Transition(float target_power, float transition_time_seconds) {
  rate_ = 1.0f / transition_time_seconds;
  target_power_ = target_power;
}
