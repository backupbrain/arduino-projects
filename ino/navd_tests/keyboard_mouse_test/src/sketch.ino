
#include <Wire.h>
#include "IQS263.h"
#include "IQS263_Init.h"
#include "IQS263_Settings.h"


#define IQS263_TOUCH_PIN A3
#define IQS263_DATA_PIN  5
#define IQS263_READY_PIN  4
#define IQS263_ADDR 0x44


#define ENCODERA 2
#define ENCODERB 3
#define ENCODERPUSH 4
#define CENTERBUTTON 7
#define RIGHTBUTTON 6
#define LEFTBUTTON 5
#define WHEELA 8
#define WHEELB 9
#define WHEELPUSH 10
#define CAPSWIPEL A0
#define CAPSWIPER A1
#define CAPPUSH A2
#define EXTRAGND A3

#define KEY_UP 0x0E
#define KEY_LEFT 0x0B
#define KEY_RIGHT 0x07
#define KEY_ENTER 0x0D
#define KEY_ESC 0x1B
#define KEY_Q 0x51
#define KEY_W 0x57
#define KEY_E 0x45
#define KEY_A 0x41
#define KEY_S 0x53
#define KEY_D 0x44
#define KEY_Z 0x5A
#define KEY_X 0x58
#define KEY_C 0x43
#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33

#include <SoftwareSerial.h>

SoftwareSerial swSerial(10, 11); // RX, TX


unsigned char data_buffer[10];
unsigned char rcv_buffer[20];


// touch pad events
bool wasTouched = false;

bool touchDown = false;
bool flickedLeft = false;
bool flickedRight = false;
bool tapped = false;
unsigned char cursorPosition;


unsigned int startPosition = 0;
unsigned int currentPosition = 0;
unsigned long touchDownTime = 0;
unsigned long touchUpTime = 0;
unsigned int touchThresholdTime = 250; //ms

void setup() {
  Serial.begin(115200);  // start serial for output

	pinMode(IQS263_TOUCH_PIN, INPUT);
  pinMode(IQS263_READY_PIN, INPUT);
  pinMode(5, OUTPUT);
  Wire.begin();        // join i2c bus (address optional for master)
  delay(5);
  IQS263_Settings();
  delay(5);//TODO: remove me later
}

void loop() {
	//Serial.println("loop");

//	Serial.println(digitalRead(IQS263_READY_PIN));
  	cursorPosition = iqs263_cursorPosition();
	//	Serial.println(cursorPosition);
	
  	delay(2);
	iqs263_touched();
	delay(2);
	iqs263_getSystemFlags();




	if (touchDown) {
		if (!wasTouched) {
			wasTouched = true;
			Serial.println("Touch Begin");
		}
		/*
		Serial.println(cursorPosition);
		if (tapped) {
			Serial.println("Tapped");
		}
		
		if (flickedRight) {
			Serial.println("Flicked Right");
		}
		if (flickedLeft) {
			Serial.println("Flicked Left");
		}
		/* */
	} else {
		if (wasTouched) {
			wasTouched = false;
			Serial.println("Touch Ended");
		}
	}
	/* */
	

	/*
  iqs263_tapped();
  iqs263_flickedRight();
  iqs263_flickedLeft();
  /* */
  delay(8);

  /*
  i2c_start();
  i2c_read(SYST_FLAGS, &rcv_buffer[0], 2); // Get Wheel Coordinates 0x02H
  delay(1);

  Serial.print("Wheel Pos: ");
  Serial.println(rcv_buffer[1]);
  /* */
}



void mouseCommand(bool button0, boolean button1, int8_t x, int8_t y) {
	uint8_t buttons = button1 << 1 | button0;

	Serial.write(0xFD);
	Serial.write((byte)0x00);
	Serial.write((byte)0x03);
	Serial.write(buttons);
	Serial.write(x);
	Serial.write(y);
	Serial.write((byte)0x00);
	Serial.write((byte)0x00);
	Serial.write((byte)0x00);
}


bool iqs263_flickedRight() {
  //bool flickedRight = false;

  //i2c_start();
  //i2c_read(SYST_FLAGS, &rcv_buffer[0], 2);

  if (rcv_buffer[1] == 255) { 
    return false;
  }

  unsigned char reading = rcv_buffer[1];
  // we want to read bit 8
  reading = reading >> 7;
  reading = reading & 0x1;

  flickedRight = false;
  if (reading) {
    //Serial.println("Flicked Right");
    flickedRight = true;
  }

  return flickedRight;
}

bool iqs263_flickedLeft() {
  //bool flickedLeft = false;

  //i2c_start();
  //i2c_read(SYST_FLAGS, &rcv_buffer[0], 2);

  if (rcv_buffer[1] == 255) { 
    return false;
  }

  unsigned char reading = rcv_buffer[1];

  // we want to read bit 7
  reading = reading >> 6;
  reading = reading & 0x1;

  flickedLeft = false;
  if (reading) {
   //Serial.println("Flicked Left");
    flickedLeft = true;
  }

  return flickedLeft;
}


unsigned int iqs263_cursorPosition() {
  i2c_start();
  i2c_read(CO_NATES,  &rcv_buffer[0], 3);
  /*
  Serial.print(rcv_buffer[0],BIN);
  Serial.print("\t");
  Serial.println(rcv_buffer[0]);
  /* */
  return rcv_buffer[0];
}


void iqs263_getSystemFlags() {
	i2c_start();
	i2c_read(SYST_FLAGS, &rcv_buffer[0], 2);

/*
	if (rcv_buffer[1] == 255) {
		rcv_buffer[1] = 0;
	}
*/
	/*
	int x = 1;
	if (rcv_buffer[x] < 128) Serial.print("0");
	if (rcv_buffer[x] < 64) Serial.print("0");
	if (rcv_buffer[x] < 32) Serial.print("0");
	if (rcv_buffer[x] < 16) Serial.print("0");
	if (rcv_buffer[x] < 8) Serial.print("0");
	if (rcv_buffer[x] < 4) Serial.print("0");
	if (rcv_buffer[x] < 2) Serial.print("0");
	Serial.print(rcv_buffer[x], BIN);
	Serial.println("");
	/* */


	//iqs263_touched();
	iqs263_tapped();
	iqs263_flickedLeft();
	iqs263_flickedRight();
}


/**
 * This one is super buggy
 * It reports anything between 0 and 10 (not touched and touched)
 * to 128 to 255
 * Currently can't determine the threshold
 */
boolean iqs263_touched(void) {
	//touchDown = !digitalRead(IQS263_TOUCH_PIN);

	//bool touched = false;

	
	i2c_start();
	i2c_read(SYST_FLAGS, &rcv_buffer[0], 2);
	
	
	int x = 1;
	if (rcv_buffer[x] < 128) Serial.print("0");
	if (rcv_buffer[x] < 64) Serial.print("0");
	if (rcv_buffer[x] < 32) Serial.print("0");
	if (rcv_buffer[x] < 16) Serial.print("0");
	if (rcv_buffer[x] < 8) Serial.print("0");
	if (rcv_buffer[x] < 4) Serial.print("0");
	if (rcv_buffer[x] < 2) Serial.print("0");
	Serial.print(rcv_buffer[x], BIN);
	Serial.println("");
	/* */


/*
	unsigned char reading = rcv_buffer[0];

	if (reading == 255) {
		reading = 0;
	}

	touchDown = false;
	if (reading > 3) {
		touchDown = true;
	}
	return touchDown;
/* */


/*
	//i2c_start();
	//i2c_read(SYST_FLAGS, &rcv_buffer[0], 2); // Get Wheel Coordinates 0x02H

  unsigned char reading = rcv_buffer[1];

  if (rcv_buffer[1] == 255) {
  	rcv_buffer[1] = 0;
  }

  // we want to read bit 3, the "slide event" bit
  reading = reading >> 1;
  reading = reading & 0x1;


  touchDown = false;
  if (reading) {
  	//Serial.println("touched");
  	touchDown = true;
  }
  return touchDown;
	/* */
}


boolean iqs263_tapped(void) {

	//bool tapped = false;
	//i2c_start();
	//i2c_read(SYST_FLAGS, &rcv_buffer[0], 2); // Get Wheel Coordinates 0x02H

  if (rcv_buffer[1] == 255) { 
    return false;
  }

  unsigned char reading = rcv_buffer[1];

  // we want to read bit 6
  reading = reading >> 5;  
  reading = reading & 0x01;

  tapped = false;
  if (reading) {
    //Serial.println("Tapped");
    tapped = true;
  }

  return tapped;
}


void IQS263_Settings(void) {
  Serial.println("Initializing Touch Sensor");

  int mode = IQS263_MODE_START;

  while (mode < IQS263_MODE_CONFIGURED) {
    switch (mode) {
      case IQS263_MODE_START:
        {

 		 Serial.println("Beginning calibration");
          mode = IQS263_MODE_AUTO_TUNING_OFF;
        }
        break;
      case IQS263_MODE_AUTO_TUNING_OFF: 
        {
          digitalWrite(IQS263_DATA_PIN, HIGH);
          delay(1);
          digitalWrite(IQS263_DATA_PIN, LOW);
          Serial.println("Setting auto tuning to off");

          unsigned char writeBufferLength = 5;
          unsigned char writeBuffer[writeBufferLength];
          writeBuffer[0] = PROX_SET0;
          writeBuffer[1] = PROX_SET1 | 0x80;
          writeBuffer[2] = PROX_SET2;
          writeBuffer[3] = PROX_SET3;
          writeBuffer[4] = EVENT_MASK;

          i2c_start();
          i2c_write(PROX_SETTINGS, &writeBuffer[0], writeBufferLength); // Prox Settingd turn ATI OFF
          
          delay(1);
          mode = IQS263_MODE_4_BIT;
        }
        break;

      case IQS263_MODE_4_BIT:
        {
          // -----------------------------------------------------
          Serial.println("Setting mode to 4 bit");

          unsigned char writeBufferLength = 1;
          unsigned char writeBuffer[writeBufferLength];
          writeBuffer[0] = (SYST_SET | 0x00);	//set proj mode through bit 4 (0x10),

          i2c_start();
          i2c_write(SYST_FLAGS, &writeBuffer[0], writeBufferLength); //

          mode = IQS263_MODE_ACTIVE_CHANNELS;
        }
        break;

      case IQS263_MODE_ACTIVE_CHANNELS:
        {

          // -----------------------------------------------------
          // FIRSTLY SETUP THE CHANNELS USED:

          // ChannelSetup Data:
          Serial.println("Setting channels");
          unsigned char writeBufferLength = 1;
          unsigned char writeBuffer[writeBufferLength];
          writeBuffer[0] = ACT_CH0 ;					// Active channels


          i2c_start();
          i2c_write(ACTIVE_CH, &writeBuffer[0], writeBufferLength);
          mode = IQS263_MODE_SET_THRESHOLD;
        }
        break;
      case IQS263_MODE_SET_THRESHOLD:
        {
          // -----------------------------------------------------
          // CONFIGURE THE THRESHOLDS:
          // Threshold settings data
          Serial.println("Configuring thresholds");
          unsigned char writeBufferLength = 8;
          unsigned char writeBuffer[writeBufferLength];
          writeBuffer[0] = PROX_THR ;				// Prox Threshold
          writeBuffer[1] = TOUCH_THR1;			// Touch Multiplier
          writeBuffer[2] = TOUCH_THR2;			// Touch mU
          writeBuffer[3] = TOUCH_THR3;			// Touch mU
          writeBuffer[4] = MOV_THR;			    // PM Prox Threshold
          writeBuffer[5] = MOV_DEB;	        // Snap threshold
          writeBuffer[6] = HALT_TIME;		    // Snap threshold
          writeBuffer[7] = I2C_TIMEOUT;		  // Non-trackpad channels prox threshold

          i2c_start();
          i2c_write(THRESHOLDS, &writeBuffer[0], writeBufferLength); // send thresholds

          mode = IQS263_MODE_SET_MULTIPLIER;
        }
        break;

      case IQS263_MODE_SET_MULTIPLIER:
        {
          // Multiplier & Base
          Serial.println("Setting multiplier");
          	unsigned char writeBufferLength = 5;
          	unsigned char writeBuffer[writeBufferLength];
             writeBuffer[0] = CH0_MULT;
             writeBuffer[1] = CH1_MULT;
             writeBuffer[2] = CH2_MULT;
             writeBuffer[3] = CH3_MULT;
             writeBuffer[4] = CH_BASE;

            i2c_start();
             i2c_write(MULTIPLERS, &writeBuffer[0], 5); // Base

             /*	writeBuffer[0] = CH0_COMP;
              	writeBuffer[1] = CH1_COMP;
              	writeBuffer[2] = CH2_COMP;
              	writeBuffer[3] = CH3_COMP;

              	this->repeat_start();
              	i2c_write(COMPENSATION, &writeBuffer[0], 4); // */


          delay(1);
          mode = IQS263_MODE_SET_AUTO_TUNING_TARGET;
        }
        break;

      case IQS263_MODE_SET_AUTO_TUNING_TARGET:
        {
          // SET ATI TARGET
          Serial.println("Setting auto-tuning target");
          unsigned char writeBufferLength = 3;
          unsigned char writeBuffer[writeBufferLength];
          writeBuffer[0] = LP_TIME ;
          writeBuffer[1] = TARGET_T;
          writeBuffer[2] = TARGET_P;

          i2c_start();
          i2c_write(TIM_TARGETS, &writeBuffer[0], writeBufferLength); // Set target


          mode = IQS263_MODE_SET_GESTURE_TIMES;
        }
        break;

      case IQS263_MODE_SET_GESTURE_TIMES:
        {
          // GESTURE TIMES
          Serial.println("Setting gesture timing");
          unsigned char writeBufferLength = 3;
          unsigned char writeBuffer[writeBufferLength];
          writeBuffer[0] = TAP_TIM ;
          writeBuffer[1] = FLICK_TIM;
          writeBuffer[2] = FLICK_THR;

          i2c_start();
          i2c_write(GESTURE_TIME, &writeBuffer[0], writeBufferLength); // Timing setting

          mode = IQS263_MODE_SET_PROXIMITY_AUTO_TUNING;
        }
        break;

      case IQS263_MODE_SET_PROXIMITY_AUTO_TUNING:
        {
          //ATI +PROX SETTINGS
          Serial.println("Setting proximity and auto tuning");
          unsigned char writeBufferLength = 5;
          unsigned char writeBuffer[writeBufferLength];
          unsigned char readBufferLength = 12;
          unsigned char readBuffer[readBufferLength];

          writeBuffer[0] = (PROX_SET0 | 0x10);
          writeBuffer[1] = (PROX_SET1) | 0xC0;
          writeBuffer[2] = (PROX_SET2) | 0x00;
          writeBuffer[3] =  PROX_SET3;
          writeBuffer[4] = (EVENT_MASK & 0xFF);


          Serial.println("-> Transmitting setting");
          i2c_start();
          i2c_write(PROX_SETTINGS, &writeBuffer[0], writeBufferLength); // AUTO ATI

          delay(100);
          delay(100);
          delay(100);


          do {
            Serial.print("-> Response: ");
            i2c_start();
            i2c_read(SYST_FLAGS, &readBuffer[0], readBufferLength); // ATI Done check
            Serial.println(readBuffer[0]);
            delay(3);
          } while (readBuffer[1] & 0x04 == 0x04);

          delay(1);
        }
        Serial.println("Init Successfully Done");
        mode = IQS263_MODE_READ;
        break;
      case IQS263_MODE_READ:
        {
          Serial.println("Reading data");
          unsigned char readBufferLength = 3;
          unsigned char readBuffer[readBufferLength];

          while (digitalRead(IQS263_READY_PIN) == HIGH)
          {
          }
          i2c_start();
          i2c_read(CO_NATES, &readBuffer[0], readBufferLength); // Get Wheel Coordinates 0x02H

          Serial.println("Wheel Value: ");
          for (unsigned char i = 0; i < 3 ; i++) {
            Serial.print(readBuffer[i]); Serial.print(", ");
          }
        }
        mode = IQS263_MODE_CONFIGURED;
        break;
    }
  }
  Serial.println("done initializing touch sensor");
}


void i2c_start() {
  //val = digitalRead(RDY_PIN );
  while (digitalRead(IQS263_READY_PIN) == 1)
  {
    //digitalWrite(IQS263_DATA_PIN, HIGH);
    __asm__("nop\n\t");
  }
  digitalWrite(IQS263_DATA_PIN, LOW);
  Wire.beginTransmission(IQS263_ADDR);
  //Wire.endTransmission();
  //delay(1);
}


void i2c_stop() {
  int ret = Wire.endTransmission();
  //Serial.print("i2c_stop returned ");Serial.println(ret);
}

void i2c_repeat() {
  int ret = Wire.endTransmission(false);
}

void i2c_write(unsigned char write_addr, unsigned char *data, unsigned char NoOfBytes) {
  unsigned char i;
  Wire.write(write_addr);
  for (i = 0 ; i < NoOfBytes ; i++)
  {
    Wire.write(data[i]);

  }
  i2c_stop();
  delay(1);
}

void i2c_read(unsigned char read_addr, unsigned char *data, unsigned char NoOfBytes) {
  unsigned char i = 0;
  i2c_send_byte(read_addr);							// IQSIQS263_DATA_PINxx address-command
  i2c_repeat(); //TODO: see into: i2c_repeat_start();
  Wire.requestFrom(IQS263_ADDR, (char)NoOfBytes);
  while (Wire.available())   // slave may send less than requested
  {
    data[i] = Wire.read(); // receive a byte as character
    i++;
  }
}



unsigned char i2c_send_byte(unsigned char c) {
  Wire.write(c);
}
