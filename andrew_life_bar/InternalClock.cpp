/**
 * InternalClock.cpp - Alarm clock library 
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */
 
#include "InternalClock.h"

/**
 * Create a clock
 * 
 * @param timeout_ms the millisecond time between alarms
 */
InternalClock::InternalClock(unsigned long timeout_ms) : _timeout_ms(timeout_ms) {
  _startTime = millis();
}

InternalClock::~InternalClock() {
}

/**
 * Change the time between alarms
 * 
 * @param timeout_ms the millisecond time between alarms
 */
void InternalClock::resetTimeout(const unsigned long timeout_ms) {
  _timeout_ms = timeout_ms;
  _startTime = millis();
}

/**
 * Trigger a function if the alarm has been triggered
 * 
 * @param onAlarmCallback a callback function to execute when the alarm is triggered
 */
void InternalClock::checkForTimeout(void (*onAlarmCallback)(void)) {
  unsigned long currentTime = millis();
  unsigned long timeDifference_ms = currentTime - _startTime;
  if (timeDifference_ms > _timeout_ms) {
    _startTime = currentTime;
    onAlarmCallback();
  }
}

