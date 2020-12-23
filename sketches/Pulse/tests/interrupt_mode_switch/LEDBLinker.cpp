#include "LEDBlinker.h"

LEDBlinker::LEDBlinker(unsigned int ledPin) {
  this->_ledPin = ledPin;
  pinMode(this->_ledPin, OUTPUT);

  // fade lookup created with this algorithm:
  // value = (int)pow(255.0 * cos((double) index<each index += 10 milliseconds> / 100*M_PI), 4);
  byte fadeLookup[] = {
    255, 254, 252, 250, 247, 242, 237, 231, 224, 216,
    208, 199, 190, 180, 170, 160, 150, 139, 129, 119, 
    109, 99, 89, 80, 72, 63, 55, 48, 42, 35, 
    30, 25, 21, 17, 13, 10, 8, 6, 4, 3, 
    2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 
    2, 3, 4, 6, 8, 10, 13, 17, 21, 25, 
    30, 35, 42, 48, 55, 63, 72, 80, 89, 99, 
    109, 119, 129, 139, 150, 160, 170, 180, 190, 199, 
    208, 216, 224, 231, 237, 242, 247, 250, 252, 254
  };
  // copy the fadelookp table to this->_fadeLookup
  // http://stackoverflow.com/a/18836190
  memcpy(this->_fadeLookup, fadeLookup, this->fadeLookupLength*sizeof(byte));
  
}
LEDBlinker::~LEDBlinker() {
  
}
void LEDBlinker::setState(unsigned int state) {
  this->_state = state;
  Serial.print("Switching to state: ");
  Serial.println(state);
  switch(this->_state) {
    case LEDBlinker::STATE_ON:
      this->_switchToOn();
    break;
    case LEDBlinker::STATE_SLOW_BLINK:
      this->_switchToSlowBlink();
    break;
    case LEDBlinker::STATE_FAST_BLINK:
      this->_switchToFastBlink();
    break;
    case LEDBlinker::STATE_FADE:
      this->_switchToFade();
    break;
    case LEDBlinker::STATE_OFF:
    default:
      this->_switchToOff();
    break;
  }
  
}

void LEDBlinker::updateAnimationFrame() {
  switch(this->_state) {
    case LEDBlinker::STATE_SLOW_BLINK:
    case LEDBlinker::STATE_FAST_BLINK:
      this->_animateBlink();
    break;
    case LEDBlinker::STATE_FADE:
      this->_animateFade();
    break;
    case LEDBlinker::STATE_ON:
    case LEDBlinker::STATE_OFF:
    default:
      this->_animateStill();
    break;
  }
}

void LEDBlinker::_switchToOff() {
  this->_blinkTime = 0;
  this->_ledState = LOW;
  this->_ledOff();
}

void LEDBlinker::_switchToOn() {
  this->_blinkTime = 0;
  this->_ledState = HIGH;
  this->_ledOn();
  
}

void LEDBlinker::_switchToSlowBlink() {
  this->_blinkTime = 500;
  this->_ledState = HIGH;
}

void LEDBlinker::_switchToFastBlink() {
  this->_blinkTime = 250;
  this->_ledState = HIGH;
}

void LEDBlinker::_switchToFade() { 
  this->_blinkTime = 0;
  this->_ledState = HIGH;
  this->_fadeAnimationStart_ts = millis();
  this->_lastFadeLookupID = 0;
}



void LEDBlinker::_animateStill() {
}

void LEDBlinker::_animateBlink() {
  unsigned long currentTime_ms = millis();
  if (currentTime_ms - this->_lastAnimationFrameUpdate_ms > this->_blinkTime) {
    this->_ledState = !this->_ledState;
    this->_lastAnimationFrameUpdate_ms = currentTime_ms;
    if (this->_ledState == HIGH) {
      this->_ledOn();
    } else {
      this->_ledOff();
    }
  }
}

void LEDBlinker::_animateFade() { 
  analogWrite(this->_ledPin, this->_fadeLookup[this->_lastFadeLookupID]);
  unsigned long currentTime_ts = millis();
  unsigned int timeDifference_ms = currentTime_ts - this->_fadeAnimationStart_ts;

  unsigned int fadePhase = timeDifference_ms % this->_fadePeriod_ms;
  // the fade lookup index represents increments of 10 ms
  this->_lastFadeLookupID = fadePhase / 10;
}




void LEDBlinker::_ledOn() { 
  digitalWrite(this->_ledPin, HIGH);
}


void LEDBlinker::_ledOff() { 
  digitalWrite(this->_ledPin, LOW);
}

