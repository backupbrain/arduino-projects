
#ifndef _FAST_ANALOG_READ_H_
#define _FAST_ANALOG_READ_H_

#include <arduino.h>

void EnableFastAnalogRead() {
  // set prescale to 16
  _SFR_BYTE(ADCSRA) &= ~_BV(ADPS2);
  _SFR_BYTE(ADCSRA) |= _BV(ADPS1);
  _SFR_BYTE(ADCSRA) |= _BV(ADPS0);
}

#endif // _FAST_ANALOG_READ_H_
