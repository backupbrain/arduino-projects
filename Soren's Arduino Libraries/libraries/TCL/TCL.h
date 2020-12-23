/*****************************************************************************
 * TCL.h
 *
 * Copyright 2011 Christpher De Vries
 * This program is distributed under the Artistic License 2.0, a copy of which
 * is included in the file LICENSE.txt
 ****************************************************************************/
#ifndef TCL_h
#define TCL_h
#include <Arduino.h>
#include <SPI.h>

#define TCL_GENERATION1 0x00
#define TCL_GENERATION2 0x01
#define TCL_GENERATION3 0x02  // LED Curtain

class TclClass {
  public:
    static void begin();
    static void end();
    static void setVersion(byte gen);
    static void sendFrame(byte flag, byte red, byte green, byte blue);
    static void sendColor(byte red, byte green, byte blue);
    static void sendDimmedColor(byte red, byte green, byte blue, byte dim);
    static void sendEmptyFrame();
    static byte makeFlag(byte red, byte green, byte blue);
  private:
    static byte version;
};

extern TclClass TCL;
#endif
