#include <Arduino.h>
#include <Wire.h>
#include "IQS263_TouchSensor.h"
void setup();
void loop();
void touchCallback();
#line 1 "src/sketch.ino"

/******************************************************
 * This is an example of the IQS263 wheel, this code 
 * is not meant for the production.* 
 * 
 ****************************************************/

//#include <Wire.h>
//#include "IQS263_TouchSensor.h"


/**
 * Touch Sensor Configuration
 */
const unsigned char touchSensor_dataPin = 5;
const unsigned char touchSensor_clockPin = 4;
const unsigned char touchSensor_readyPin = 4;
const unsigned char touchSensor_slaveAddress = 0x44;


#define I2C_DIRECTION_TX 0x00
#define STOP_CHANNEL     true
#define NUM_BYTES        3



unsigned char moveThreshold = 5;

unsigned char lastTouchPosition = 0;
unsigned char movedBy = 0;
bool touched = false;

unsigned char rcv_buffer[20];
unsigned char val;
unsigned char wheel;

/**
 * System configuration
 */
unsigned long currentTime;
unsigned long frameTimeout = 0;
unsigned int frameDuration = 50;

IQS263_TouchSensor touchSensor = IQS263_TouchSensor(
	touchSensor_dataPin,
	touchSensor_clockPin,
	touchSensor_readyPin,
	touchSensor_slaveAddress
);

void setup() {

  	Serial.begin(115200);  // start serial for output
  	/*
	Serial.flush(); //flush all previous received and transmitted data
	while(!Serial.available());
	/* */

	Serial.print(F("Initializing IQS263 Touch Sensor..."));
	touchSensor.initialize();
	Serial.println(F("done"));


}

void loop() {
	unsigned char currentTouchPosition = 0;


	currentTime = millis();
 	if (currentTime > frameTimeout) {




 		touchSensor.isTouched();


 		//touchSensor.getPosition();

		//touchSensor.isFlickedRight();
		//touchSensor.isFlickedLeft();
		//touchSensor.isTapped();
 		

 		/*
 			Serial.print("System Flags: ");
			touchSensor.read(0x01,  rcv_buffer, 2);
			for (int x=1; x<2; x++) {
				if (rcv_buffer[x] < 128) Serial.print("0");
				if (rcv_buffer[x] < 64) Serial.print("0");
				if (rcv_buffer[x] < 32) Serial.print("0");
				if (rcv_buffer[x] < 16) Serial.print("0");
				if (rcv_buffer[x] < 8) Serial.print("0");
				if (rcv_buffer[x] < 4) Serial.print("0");
				if (rcv_buffer[x] < 2) Serial.print("0");
				Serial.print(rcv_buffer[x], BIN);
			}
	 		Serial.print(" ");



 			Serial.print("Wheel Coords: ");
			touchSensor.read(0x01,  rcv_buffer, 3);
			for (int x=0; x<3; x++) {
				if (rcv_buffer[x] < 128) Serial.print("0");
				if (rcv_buffer[x] < 64) Serial.print("0");
				if (rcv_buffer[x] < 32) Serial.print("0");
				if (rcv_buffer[x] < 16) Serial.print("0");
				if (rcv_buffer[x] < 8) Serial.print("0");
				if (rcv_buffer[x] < 4) Serial.print("0");
				if (rcv_buffer[x] < 2) Serial.print("0");
				Serial.print(rcv_buffer[x], BIN);
			}
	 		Serial.println("");
/* */



 		//Serial.println("Main loop");

 		/*
		currentTouchPosition = touchSensor.getPosition();
 		if (currentTouchPosition > 5) {
 			lastTouchPosition = currentTouchPosition;
 		}
		//Serial.print("Wheel Pos: ");
		Serial.println(lastTouchPosition);
 		/*  */

		/*
		if (touchSensor.isTouched()) {
			if (!touched) {
				touched = true;
				Serial.println("==============================");
				Serial.println("         Touch Started        ");
				Serial.println("------------------------------");
			}
			currentTouchPosition = touchSensor.getPosition();
	 		if (currentTouchPosition > 5) {
	 			movedBy = currentTouchPosition - lastTouchPosition;
	 			lastTouchPosition = currentTouchPosition;
	 		}

	 		Serial.print("Position: ");
	 		Serial.print(currentTouchPosition);
	 		Serial.print(", \t Moved By: ");
	 		Serial.println(movedBy);

		} else {
			if (touched) {
				touched = false;
				Serial.println("------------------------------");
				Serial.println("          Touch Ended         ");
				Serial.println("==============================");
			}
		}
		/* */




 		frameTimeout = currentTime + frameDuration;
	}


}


void touchCallback() {

}