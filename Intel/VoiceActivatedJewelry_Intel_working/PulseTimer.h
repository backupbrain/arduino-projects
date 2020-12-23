#include <Arduino.h>

#ifndef PulseTimer_h
#define PulseTimer_h

// Utility classes from ZackeesFashionGlowJewelry_v1.h (omitted)
class PulseTimer {
  public:
    PulseTimer();
    void setDelay(unsigned long delay);
    bool Update();
  private:
    unsigned long mDelay;
    unsigned long mStartTime;
};
PulseTimer::PulseTimer() {
  mStartTime = millis();
}
void PulseTimer::setDelay(unsigned long delay) {
  mDelay = delay;
}
bool PulseTimer::Update() {
  unsigned long now_ms = millis();
  unsigned long delta = now_ms - mStartTime;
  bool output = false;
  if (delta > mDelay) {
    output = true;
    mStartTime = now_ms;
  }
  return output;
}


#endif

