/**
 * Shift Registers that allow us to pinpoint which keys have been pressed
 *
 * We are looking for pin combinations, such as  
 */


 /*

 Table stolen from:
 http://www.instructables.com/id/Hacking-a-USB-Keyboard/step3/A-note-on-pin-layouts-and-shift-registers/



 		A  1    2   3     4   5    6    7     8    9    A     B    C   D    E   F   
	B	0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111
xxxx0001 
2	0010   1         Z             A          Q
3	0011   2             X         S          W
4	0100   3             C         D          E 
5	0101   4    5   B    V    G    F     T    R
6	0110   7    6   N    M    H    J     Y    U
7	0111   8             ,         K          I
8	1000   9             .         L          O
9	1001   0    -   /              ;          P
A	1010   
B	1011               ENTR       " "
C	1100 
D	1101 
E	1110 
F	1111 

 */

#ifndef ShiftRegister_h
#define ShiftRegister_h

#include "Keyboard.h"


const byte shiftRegisterValues[NUM_CHARS];

const byte shiftRegisterValues[CHAR_COMMA] = 0x47;
const byte shiftRegisterValues[CHAR_PERIOD] = 0x48;
const byte shiftRegisterValues[CHAR_FORWARD_SLASH] = 0x39;
const byte shiftRegisterValues[CHAR_DASH] = 0x29;
const byte shiftRegisterValues[CHAR_SEMICOLON] = 0x69;

const byte shiftRegisterValues[CHAR_ENTER] = 0x4B;
const byte shiftRegisterValues[CHAR_SPACE] = 0x6B;

const byte shiftRegisterValues[CHAR_A] = 0x62;
const byte shiftRegisterValues[CHAR_B] = 0x35;
const byte shiftRegisterValues[CHAR_C] = 0x43;
const byte shiftRegisterValues[CHAR_D] = 0x64;
const byte shiftRegisterValues[CHAR_E] = 0x84;
const byte shiftRegisterValues[CHAR_F] = 0x65;
const byte shiftRegisterValues[CHAR_G] = 0x55;
const byte shiftRegisterValues[CHAR_H] = 0x56;
const byte shiftRegisterValues[CHAR_I] = 0x87;
const byte shiftRegisterValues[CHAR_J] = 0x66;
const byte shiftRegisterValues[CHAR_K] = 0x67;
const byte shiftRegisterValues[CHAR_L] = 0x68;
const byte shiftRegisterValues[CHAR_M] = 0x46;
const byte shiftRegisterValues[CHAR_N] = 0x36;
const byte shiftRegisterValues[CHAR_O] = 0x88;
const byte shiftRegisterValues[CHAR_P] = 0x89;
const byte shiftRegisterValues[CHAR_Q] = 0x82;
const byte shiftRegisterValues[CHAR_R] = 0x85;
const byte shiftRegisterValues[CHAR_S] = 0x63;
const byte shiftRegisterValues[CHAR_T] = 0x75;
const byte shiftRegisterValues[CHAR_U] = 0x86;
const byte shiftRegisterValues[CHAR_V] = 0x45;
const byte shiftRegisterValues[CHAR_W] = 0x83;
const byte shiftRegisterValues[CHAR_X] = 0x83;
const byte shiftRegisterValues[CHAR_Y] = 0x76;
const byte shiftRegisterValues[CHAR_Z] = 0x32;

const byte shiftRegisterValues[CHAR_1] = 0x12;
const byte shiftRegisterValues[CHAR_2] = 0x13;
const byte shiftRegisterValues[CHAR_3] = 0x14;
const byte shiftRegisterValues[CHAR_4] = 0x15;
const byte shiftRegisterValues[CHAR_5] = 0x25;
const byte shiftRegisterValues[CHAR_6] = 0x35;
const byte shiftRegisterValues[CHAR_7] = 0x16;
const byte shiftRegisterValues[CHAR_8] = 0x17;
const byte shiftRegisterValues[CHAR_9] = 0x18;
const byte shiftRegisterValues[CHAR_0] = 0x19;






#endif