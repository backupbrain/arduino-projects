/**
 * InternalClock.h - Alarm clock library 
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */
 
#ifndef InternalClock_h
#define InternalClock_h

#include <Arduino.h>

class InternalClock {
  public:
    // constructors
    InternalClock(const unsigned long timeout_ms);
    ~InternalClock();

    // controllers
    void checkForTimeout(void (*onAlarmCallback)(void));
    void resetTimeout(unsigned long timeout_ms);

  private:
    // internal state
    unsigned long _startTime;
    unsigned long _timeout_ms;
};

#endif
