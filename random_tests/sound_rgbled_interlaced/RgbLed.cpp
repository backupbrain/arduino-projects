/*
  RgbLED.cpp - Library for outputting color to a 3 color LED
  Created by Tony Gaitatzis, June 17, 2015.
  Released into the public domain.
*/
#include "RgbLED.h"


RgbLED::RgbLED(char port, boolean ledType) {
  this->_ledType = ledType;
  
  // set the ports: B: pins 8-13 on ardunio, D: 0-7 on arduino
  //if (port == 'B') {
    this->_port = PORTB;
    this->_ddr = DDRB;
  //} else if (port == 'D') {
  //  this->_port = PORTD;
  //  this->_ddr = DDRD;
  //}
  
  // on trinket, want to use pins 0, 1, and 4 for pwm
  //this->_ddr = this->_ddr | B00010011;
  DDRB = DDRB | B00000111;
  
}

RgbLED::~RgbLED() {
}



/**
 * set the new color
 *
 * usage: rgbLED.setColor(rgbColor);
 * returns void
 */
void RgbLED::setColor(RgbColor rgbColor) {
  this->_rgbColor = rgbColor;
}
/**
 * lights the LED with the current color
 *
 * usage: rgbLED.output();
 * returns void
 */
void RgbLED::output() {
  if (this->_ledType == LED_TYPE_COMMON_CATHODE) {
    //this->_pwm(128, 0, 32);
    this->_pwm(this->_rgbColor.r, this->_rgbColor.g, this->_rgbColor.b, 0, 255);
    /*
    analogWrite(this->_rPin, this->_rgbColor.r);
    analogWrite(this->_gPin, this->_rgbColor.g);
    analogWrite(this->_bPin, this->_rgbColor.b);
    */
  } else {
    //this->_pwm(128, 0, 32);
    this->_pwm(255-this->_rgbColor.r, 255-this->_rgbColor.g, 255-this->_rgbColor.b, 0, 255);
    /*
    analogWrite(this->_rPin, 255-this->_rgbColor.r);
    analogWrite(this->_gPin, 255-this->_rgbColor.g);
    analogWrite(this->_bPin, 255-this->_rgbColor.b);
    */
  }
}
  
/**
 * return the current color
 *
 * usage: rgbLED.getColor();
 * returns RgbColor
 */
RgbColor RgbLED::getColor() {
  return this->_rgbColor;
}






void RgbLED::_pwm(byte r, byte g, byte b, byte startPoint, byte endPoint) {
      // assume port 0 is red
      // assume port 1 is green
      // assume port 4 is blue
      
  // 1/2 microseconds per cycle
  
  unsigned int cycles = 255;
  unsigned int onCycles_r = cycles*r/255;
  unsigned int onCycles_g = cycles*g/255;
  unsigned int onCycles_b = cycles*b/255;
  
  unsigned int duration_us = 1000;
  unsigned int currentTime_us = micros();
  unsigned int startTime_us = currentTime_us;
  unsigned int timeDifference_us = currentTime_us - startTime_us;
  
  unsigned int currentCyclePosition;
  
  unsigned long startState_r = B00000001;
  unsigned long startState_g = B00000010;
  unsigned long startState_b = B00000100;
  unsigned long offState = B00000000;
  
  unsigned long currentState_r;
  unsigned long currentState_g;
  unsigned long currentState_b;
  
  unsigned int masterCounter = startPoint;
  unsigned int numIterations = endPoint - startPoint;
  cli(); 
  while (masterCounter++ < numIterations) { 
  //while (timeDifference_us < duration_us) {
    currentTime_us = micros();
    currentCyclePosition = startPoint;
    
    currentState_r = startState_r;
    currentState_g = startState_g;
    currentState_b = startState_b;
    
    while (currentCyclePosition++ < cycles) {
      if (currentState_r == startState_r) {
        if (currentCyclePosition > onCycles_r) {
          currentState_r = offState;
        }
      }
      if (currentState_g == startState_g) {
        if (currentCyclePosition > onCycles_g) {
          currentState_g = offState;
        }
      }
      if (currentState_b == startState_b) {
        if (currentCyclePosition > onCycles_b) {
          currentState_b = offState;
        }
      }
      
      PORTB = offState | currentState_b | currentState_g | currentState_r; 
      
      //Serial.println(PORTB, BIN);
    
      timeDifference_us = currentTime_us - startTime_us;
    }
  }
  sei();
}

