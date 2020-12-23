
struct PulseTimer {
  unsigned long mDelay;
  unsigned long mStartTime;
  PulseTimer(unsigned long delay) {
    mStartTime = millis();
    mDelay = delay;
  }
  
  // Returns true if update triggered a pulse.
  bool update() {
    unsigned long now_ms = millis();
    unsigned long delta = now_ms - mStartTime;
    bool output = false;
    if (delta > mDelay) {
      output = true;
      mStartTime = now_ms;
    }
    return output;
  }
};
