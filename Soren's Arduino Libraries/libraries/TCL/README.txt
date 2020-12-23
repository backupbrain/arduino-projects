Arduino Total Control Lighting Library
Copyright 2011 Christopher De Vries

This library allows you to control Cool Neon's Total Control Lighting using an
Arduino. The library is built on SPI and requires that the Arduino SPI library
also be included.

Installation
------------
To install this library, unzip it into the "libraries" directory of your
Arduino sketchbook folder. If there is not already a folder named "libraries"
then create it. 

Hardware
--------
The Total Control Lighting strands use 4 wires to provide both power and
communication. If you are using the Cool Neon connectors, the wires are
designated with the following colors.

Red:    +5V
Yellow: Clock
Green:  Data
Blue:   Ground

If you are using a small number of LEDs the Arduino itself can provide power
to the strand, but for a significant number of lights (greater than 5) an
external power source should be attached. The Arduino will control the lights
using the Clock and Data wires.

The Clock wire should be connected to the SPI clock out of the Arduino. For an
Arduino UNO this is digital IO Pin 13, for an Arduino Mega this is digital IO
pin 52. The Data line should be connected to the SPI MOSI line, which for an
Arduino UNO is digital IO Pin 11, and for an Arduino Mega is digital IO pin
51. Be sure there is a common ground between the Arduino and the Total Control
Lighting and your LED strand should be ready to receive data.

Software
--------
In order to use the library, your sketch should start with the following
#include directives:

#include <SPI.h>
#include <TCL.h>

There are two generations of TCL lighting available. The first generation
lighting receives red, green, and blue pixel colors as bytes varying from 0
(off) to 255 (fully on). The second generation also has the ability to dim the
pixel by a dimming factor which is a byte that varies from 0 (off) to 31
(fully on). The protocol for each is a bit different, so you must select the
generation using the command TCL.setVersion(byte gen) command.

By default the version is the first generation. You can select the first
generation protocol by issuing the command:

TCL.setVersion(TCL_GENERATION1);

You can select the second generation protocol by issuing the command:

TCL.setVersion(TCL_GENERATION2);

Before starting communication, initialize the SPI bus by issuing the command:

TCL.begin();

Before sending a group of colors to your LEDs, you must send 4 bytes of 0s to
initialize the strand. You can do this using the command:

TCL.sendEmptyFrame();

You must also do this after every set of colors is sent to the strand.

To send an RGB color to either a generation 1 or generation 2 strand, use the command:

TCL.sendColor(byte red, byte green, byte blue);

So, for example to send a yellow pixel, I would issue the command:

TCL.sendColor(0xff,0xb0,0x00);

The first color you send, will be the color of the first pixel (closest to the
Arduino), the next will be the next pixel and so on down the line.

On a second generation TCL strand you can also send a dimmed RGB color. To
send a dimmed color use the command:

TCL.sendDimmedColor(byte red, byte green, byte blue, byte dim);

where red is in the range 0-255, green is in the range 0-255, blue is in the
range 0-255, and dim is in the range 0-31. For example, to send a 50% dimmed
blue pixel I would issue the command:

TCL.sendDimmedColor(0,0,255,15);

Please remember that only the second generation TCL strands accept dimmed
colors. First generation strands will not respond to these commands.

When you are done sending colors, remember to finish with

TCL.sendEmptyFrame();

