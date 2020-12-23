/*
  IQS263_TouchSensor.h - Library for using the Azoteq IQS263,
  an I2C compatible digital touch sensor module.
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/

#ifndef IQS263_TouchSensor_h
#define IQS263_TouchSensor_h

#include "Arduino.h"
#include <Wire.h>
#include "IQS263.h"
#include "IQS263_Init.h"



#define IQS263_MODE_START 0
#define IQS263_MODE_AUTO_TUNING_OFF 1
#define IQS263_MODE_4_BIT 2
#define IQS263_MODE_ACTIVE_CHANNELS 3
#define IQS263_MODE_SET_THRESHOLD 4
#define IQS263_MODE_SET_MULTIPLIER 5
#define IQS263_MODE_SET_AUTO_TUNING_TARGET 6
#define IQS263_MODE_SET_GESTURE_TIMES 7
#define IQS263_MODE_SET_PROXIMITY_AUTO_TUNING 8
#define IQS263_MODE_READ 9
#define IQS263_MODE_CONFIGURED 0

#define IQS263_TOUCH_THRESHOLD 4

class IQS263_TouchSensor
{
  public:    

  	enum Constants {
  		TOUCH_SENSOR_THRESHOLD = 4,
  		PRODUCT_NUMBER = 0x3C,
  		SYSTEM_FLAGS = 0x01
  	};

   	/**
   	 * Constructor
   	 */
    IQS263_TouchSensor(unsigned char dataPin, unsigned char clockPin, unsigned char readyPin, unsigned char address);

   	/**
   	 * Destructor
   	 */
    ~IQS263_TouchSensor();  
    


   	/**
   	 * Initialize the sensor
   	 */
    void initialize();


	bool isFlickedLeft();
	bool isFlickedRight();
	bool isTapped();
    /**
     * Get the last touch position
     */
   	unsigned int getPosition();

    /**
     * Get the touch status
     */
   	bool isTouched();


   	/**
   	 * Start the I2C bus Communication
   	 */
	void startTransmission();

   	/**
   	 * Stop the I2C bus Communication
   	 */
	void stopTransmission();

   	/**
   	 * FIXME: Unknown
   	 */
	void repeat() ;

   	/**
   	 * Write an array of byte to the address
   	 */
	void write(unsigned char write_addr, unsigned char *data, unsigned char NoOfBytes) ;


   	/**
   	 * Read an array of byte to the address
   	 */
	void read(unsigned char read_addr, unsigned char *data, unsigned char NoOfBytes);

   	/**
   	 * Send a single byte
   	 */
	unsigned char sendByte(unsigned char c) ;



  private:
  	unsigned char _dataPin;
  	unsigned char _clockPin;
  	unsigned char _readyPin;
  	unsigned char _address;
    
};

#endif
