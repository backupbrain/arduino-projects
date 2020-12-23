/**
 * Heartbeat.h - Animates a blinking LED to show the Arduino is working
 * Created by Tony Gaitatzis, 2018
 * Released into the public domain.
 */
 
#ifndef Heartbeat_h
#define Heartbeat_h
#include <Arduino.h>

class Heartbeat {
  public:
    // constructor
    Heartbeat(const unsigned int heartbeatLedPin);
    ~Heartbeat();

    // controller
    void beat();

  private:
    // internal configuration
    const unsigned int _heartbeatLedPin;
};

#endif
