/**
 * This LED blinker can be programmed to support several LED blink states.
 * It is designed to act like a typical LED blinker found on Christmast lights
 * 
 * @author Tony Gaitatzis tonyg@tonygaitatzis.com
 * @version 1.0  2015/08/01
 */
 
#ifndef LEDBlinker_h
#define LEDBlinker_h

#include <Arduino.h>

class LEDBlinker {
  public:
    static const unsigned int STATE_OFF = 0;
    static const unsigned int STATE_ON = 1;
    static const unsigned int STATE_SLOW_BLINK = 2;
    static const unsigned int STATE_FAST_BLINK = 3;
    static const unsigned int STATE_FADE = 4;
    
    static const unsigned int fadeLookupLength = 100;
    
    static const unsigned int numStates = 5;
    
    LEDBlinker(unsigned int ledPin);
    ~LEDBlinker();
    void setState(unsigned int state);
    void updateAnimationFrame();
  private:
    byte _fadeLookup[LEDBlinker::fadeLookupLength];
    static const unsigned int _fadePeriod_ms = 1000;
    unsigned int _state;
    unsigned int _ledPin;
    bool _ledState;
    unsigned int _blinkTime;
    unsigned long _lastAnimationFrameUpdate_ms;
    unsigned long _fadeAnimationStart_ts;
    unsigned int _lastFadeLookupID;

    byte _currentFadePWM;
    bool _fadeIncreasing;
    
    void _switchToOff();
    void _switchToOn();
    void _switchToSlowBlink();
    void _switchToFastBlink();
    void _switchToFade();
    
    void _animateStill();
    void _animateBlink();
    void _animateFade();

    void _ledOn();
    void _ledOff();
};

#endif
