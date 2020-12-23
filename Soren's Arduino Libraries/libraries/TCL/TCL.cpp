/*****************************************************************************
 * Tcl.cpp
 *
 * Copyright 2011 Christpher De Vries
 * This program is distributed under the Artistic License 2.0, a copy of which
 * is included in the file LICENSE.txt
 ****************************************************************************/

#include "Arduino.h"
#include "SPI.h"
#include "TCL.h"

TclClass TCL;
byte TclClass::version = TCL_GENERATION1;

void TclClass::begin() {
  // Set the SPI parameters
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void TclClass::end() {
  SPI.end();
}

void TclClass::setVersion(byte gen) {
  version = gen;
}

byte TclClass::makeFlag(byte red, byte green, byte blue) {
  byte flag = 0;

  flag = (red&0xC0)>>6;
  flag |= ((green&0xC0)>>4);
  flag |= ((blue&0xC0)>>2);
  return ~flag;
}

void TclClass::sendFrame(byte flag, byte red, byte green, byte blue) {
  SPI.transfer(flag);
  SPI.transfer(blue);
  SPI.transfer(green);
  SPI.transfer(red);
}

void TclClass::sendColor(byte red, byte green, byte blue) {
  byte flag;

  if(version==TCL_GENERATION1) {
    flag = makeFlag(red,green,blue);
  }
  else if(version==TCL_GENERATION2) {
    flag = 0xff;
  }

  sendFrame(flag,red,green,blue);
}

void TclClass::sendDimmedColor(byte red, byte green, byte blue, byte dim) {
  byte flag = 0xe0|dim;

  sendFrame(flag,red,green,blue);
}

void TclClass::sendEmptyFrame() {
  sendFrame(0x00,0x00,0x00,0x00);
}

