/**
 * Keyboard.h
 * A shift-register based keyboard driver
 * for a home-brew Arduino keyboard
 * 
 * outputs serial ascii characters
 */

#ifndef Keyboard_h
#define Keyboard_h

#include "Arduino.h"

#include "KeyboardSettings.h"
#include "Keycodes.h"
#include "ShiftRegister.h"

class Keyboard {
public:
	Keyboard();
	bool pollState();
	char getCharPressed();
private:


	static const byte _shiftRegisterBitMask = 0b11110000;

	static const byte _numShiftRegisterPins = 17;  // number of shift register pins
	byte _shiftRegisterPins[numShiftRegisterPins];	// shift registers




	static const byte _shiftRegisterOffsetStart = 9; // split the shift registers into two groups

	// record the dimensions of the shiftRegisters
	enum shiftRegisterPoints {
		LOW_START = 0,
		LOW_END = 1,
		LOW_LENGTH = 2,
		HIGH_START = 3,
		HIGH_END = 4,
		HIGH_LENGTH = 5

	}
	/*
	static const byte _LOW_START = 0,
		_LOW_END = 1,
		_LOW_LENGTH = 2,
		_HIGH_START = 3,
		_HIGH_END = 4,
		_HIGH_LENGTH = 5;
	/* */
	byte _shiftRegisterDimensions[6];


	byte _currentShiftRegisterValues[numShiftRegisterPins];	// shift register values
	byte _previousShiftRegisterValues[numShiftRegisterPins];	// last shift registers

	char _getCharFromRegisterValue(byte shiftRegisterValue);
};



#endif