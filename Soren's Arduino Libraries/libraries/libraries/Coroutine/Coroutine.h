#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include <Arduino.h>

///////////////////////////////////////////////////////////////////////////////
class AbstractCoroutine {
 public:
  AbstractCoroutine();
  virtual ~AbstractCoroutine();

  int Update(int delta_time_ms);
  // Interface function
  // Returns the amount of time to sleep in milliseconds
  // before the next OnCoroutine() is called.
  virtual int OnCoroutine() = 0;

 private:
  int time_trigger_ms_;
};

///////////////////////////////////////////////////////////////////////////////
class CoroutineDispatch {
 public:
  CoroutineDispatch(AbstractCoroutine** coroutines, int num_coroutines) {
    Init(coroutines, num_coroutines);
  }

  template <int N>
  CoroutineDispatch(AbstractCoroutine* (&coroutines)[N]) {
    Init(coroutines, N);
  }

  // Returns the delay in ms that the caller can wait before
  // calling CoroutineDispatch again. This return value is optional and is
  // meant to preserve battery life by allowing the maximum amount of cpu
  // sleep.
  int Update();

 private:
  void Init(AbstractCoroutine** coroutines, int num_coroutines);

  AbstractCoroutine** coroutines_;
  int num_coroutines_;
  int last_time_ms_;
};

#endif  // __COROUTINE_H__
