#include "Coroutine.h"

AbstractCoroutine::AbstractCoroutine() : time_trigger_ms_(0.0f) {}
AbstractCoroutine::~AbstractCoroutine() {}

int AbstractCoroutine::Update(int delta_time_ms) {
  time_trigger_ms_ -= delta_time_ms;
  if (time_trigger_ms_ < 0.0) {
    time_trigger_ms_ = OnCoroutine();
  }
}


// Returns the time in milliseconds that this object should be called back.
int CoroutineDispatch::Update() {
  int curr_time_ms = millis();
  // Either first run condition OR time wrapped OR called in same millisecond
  // slice.
  if (last_time_ms_ == 0.0f || last_time_ms_ >= curr_time_ms) {
    // OnRoutine() is not called but the time is updated.
    last_time_ms_ = curr_time_ms;
    return 1.0f;  // Wait 1 ms before being called again.
  }
  // dt from last invokation.
  int dt = curr_time_ms - last_time_ms_;
  last_time_ms_ = curr_time_ms;

  int min_delay_ms = 1000.0f;  // 1 second delay max.
  for (int i = 0; i < num_coroutines_; ++i) {
    AbstractCoroutine* coroutine = coroutines_[i];
    int rtn_delay_ms = coroutine->Update(dt);
    min_delay_ms = min(min_delay_ms, rtn_delay_ms);
  }

  return max(1, min_delay_ms);  // Sleep at least one ms.
}

void CoroutineDispatch::Init(AbstractCoroutine** coroutines, int num_coroutines) {
   coroutines_ = coroutines;
   num_coroutines_ = num_coroutines;
   last_time_ms_ = 0.0f;
}
