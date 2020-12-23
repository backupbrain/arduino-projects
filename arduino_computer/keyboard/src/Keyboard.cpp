/**
 * Keyboard.cpp
 * A shift-register based keyboard driver
 * for a home-brew Arduino keyboard
 * 
 * outputs serial ascii characters
 */

/**
 * Instantiate the keyboard
 */
 Keyboard::Keyboard() {
	// set up the shift register dimensions
	this->_shiftRegisterDimensions[this->LOW_START] = 0;
	this->_shiftRegisterDimensions[this->LOW_END] = this->_shiftRegisterOffsetStart - 1;
	this->_shiftRegisterDimensions[this->LOW_LENGTH] = this->_shiftRegisterOffsetStart;
	this->_shiftRegisterDimensions[this->HIGH_START] = this->_shiftRegisterOffsetStart;
	this->_shiftRegisterDimensions[this->HIGH_END] = this->_numShiftRegisterPins - 1;
	this->_shiftRegisterDimensions[this->HIGH_LENGTH] = this->_numShiftRegisterPins - this->_shiftRegisterOffsetStart;

	// there is a better way to do this, possibly with an enum table, but for now it's here
	// this creates an array of pinouts
	this->_shiftRegisterPins[0] = 0;
	this->_shiftRegisterPins[1] = 1;
	this->_shiftRegisterPins[2] = 2;
	this->_shiftRegisterPins[3] = 3;
	this->_shiftRegisterPins[4] = 4;
	this->_shiftRegisterPins[5] = 5;
	this->_shiftRegisterPins[6] = 6;
	this->_shiftRegisterPins[7] = 7;
	this->_shiftRegisterPins[7] = 8;
	this->_shiftRegisterPins[8] = 8;
	this->_shiftRegisterPins[9] = 9;
	this->_shiftRegisterPins[10] = 10;
	this->_shiftRegisterPins[11] = 11;
	this->_shiftRegisterPins[12] = 12;
	this->_shiftRegisterPins[13] = 13;
	this->_shiftRegisterPins[14] = A0;
	this->_shiftRegisterPins[15] = A1;
	this->_shiftRegisterPins[16] = A2;
	this->_shiftRegisterPins[17] = A3;

	// map each of the shift register pins as inputs
	for (int pin=0; pin < this->_numShiftRegisterPins; pin++) {
		pinMode(pin, INPUT);
	}
 }


/**
 * Poll the register pins to determine if a key has been pressed
 *
 * @access public
 */
bool Keyboard::pollState() {
	bool stateChanged = false;
	for (int pinID = 0; pinID < this->_numShiftRegisterPins, pinID++) {
		// assume for now that all shift registers are pullUp,
		pinValue = digitalRead(shiftRegisters[pinID]);
		// handle debounce and only record rising edge
		if (currentShiftRegisterValues[pinID] > previousShiftRegisterValues[pinID]) {
			stateChanged = true;
		}
		previousShiftRegisterValues[pinID] = currentShiftRegisterValues[pinID];
	}
	return stateChanged;
}


void Keyboard::getCharPressed() {
	char charPressed = CHAR_NULL;
	// determine the current shift register layout and convert to ascii
	byte aPinID, bPinID;
	bool aPinSet = false;
	bool bPinSet = false;

	for (int pinID = this->_shiftRegisterDimensions[this->LOW_START]; pinID <= this->_shiftRegisterDimensions[this->LOW_END]; pinID++) {
		if (this->_currentShiftRegisterValues[pinID] == true) {
			aPinID = pinID;
			aPinSet = true;
			break;
		}
	}

	for (int pinID = shiftRegisterDimensions[this->LOW_START]; pinID <= shiftRegisterDimensions[this->LOW_END]; pinID++) {
		if (this->_currentShiftRegisterValues[pinID] == true) {
			bPinID = pinID;
			bPinSet = true;
			break;
		}
	}


	// assemble the registers
	byte aRegister, // low
		bRegister;	// high
	byte registerValue;
	if (aPinSet && bPinSet) {
		aRegister = aPinID;

		bRegister = bPinID - this->_shiftRegisterDimensions[this->HIGH_START];
		bRegister = bRegister << 4;
		bRegister = bRegister & this->_shiftRegisterBitMask;

		registerValue = bRegister | aRegister;

		charPressed = this->_getCharFromRegisterValue(registerValue);
	}

	return charPressed;

}


/**
 * Handle a key press event
 *
 * @access private
 * @parameter byte registerValue the shiftRegister value that was pressed
 */
char Keyboard::_getCharFromRegisterValue(byte registerValue) {
	byte charID = 0;
	byte asciiCharacter = CHAR_NULL;

	// map the register to an ascii character and output somewhere, somehow
	// start by performing a reverse lookup on the register
	for (int id = 0; id < NUM_CHARS; id++) {
		if (this->_shiftRegisterValues[id] == registerValue) {
			charID = id;
		}
	}

	asciiCharacter = asciiTable[charID];

	return asciiCharacter;

}

