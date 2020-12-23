#include "IQS263_TouchSensor.h"



IQS263_TouchSensor::IQS263_TouchSensor(unsigned char dataPin, unsigned char clockPin, unsigned char readyPin, unsigned char address) { 
  this->_dataPin = dataPin;
  this->_clockPin = clockPin;
  this->_readyPin = readyPin;
  this->_address = address;


  pinMode(this->_readyPin, INPUT);
  pinMode(this->_dataPin, OUTPUT);

}



IQS263_TouchSensor::~IQS263_TouchSensor() {
}



void IQS263_TouchSensor::initialize() {
  Serial.println("Initializing Touch Sensor");

  unsigned char mode = IQS263_MODE_START;

  while (mode < IQS263_MODE_CONFIGURED) {
    switch (mode) {
      case IQS263_MODE_START:
        {
          mode = IQS263_MODE_AUTO_TUNING_OFF;
        }
        break;
      case IQS263_MODE_AUTO_TUNING_OFF: 
        {
          digitalWrite(this->_dataPin, HIGH);
          delay(1);
          digitalWrite(this->_dataPin, LOW);
          Serial.println("Setting auto tuning to off");

          unsigned char writeBufferLength = 5;
          unsigned char writeBuffer[writeBufferLength];
          writeBuffer[0] = PROX_SET0;
          writeBuffer[1] = PROX_SET1 | 0x80;
          writeBuffer[2] = PROX_SET2;
          writeBuffer[3] = PROX_SET3;
          writeBuffer[4] = EVENT_MASK;

          this->startTransmission();
          this->write(PROX_SETTINGS, &writeBuffer[0], writeBufferLength); // Prox Settingd turn ATI OFF
          
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

          this->startTransmission();
          this->write(SYST_FLAGS, &writeBuffer[0], writeBufferLength); //

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


          this->startTransmission();
          this->write(ACTIVE_CH, &writeBuffer[0], writeBufferLength);
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

          this->startTransmission();
          this->write(THRESHOLDS, &writeBuffer[0], writeBufferLength); // send thresholds

          mode = IQS263_MODE_SET_MULTIPLIER;
        }
        break;

      case IQS263_MODE_SET_MULTIPLIER:
        {
          // Multiplier & Base
          Serial.println("Setting multiplier");
          /*    writeBuffer[0] = CH0_MULT ;
             writeBuffer[1] = CH1_MULT;
             writeBuffer[2] = CH2_MULT;
             writeBuffer[3] = CH3_MULT;
             writeBuffer[4] = CH_BASE;

            this->startTransmission();
             this->write(MULTIPLERS, &writeBuffer[0], 5); // Base

             /*	writeBuffer[0] = CH0_COMP;
              	writeBuffer[1] = CH1_COMP;
              	writeBuffer[2] = CH2_COMP;
              	writeBuffer[3] = CH3_COMP;

              	this->repeat_start();
              	this->write(COMPENSATION, &writeBuffer[0], 4); // */


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

          this->startTransmission();
          this->write(TIM_TARGETS, &writeBuffer[0], writeBufferLength); // Set target


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

          this->startTransmission();
          this->write(GESTURE_TIME, &writeBuffer[0], writeBufferLength); // Timing setting

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
          this->startTransmission();
          this->write(PROX_SETTINGS, &writeBuffer[0], writeBufferLength); // AUTO ATI

          delay(100);
          delay(100);
          delay(100);


          do {
            Serial.print("-> Response: ");
            this->startTransmission();
            this->read(SYST_FLAGS, &readBuffer[0], readBufferLength); // ATI Done check
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

          while (digitalRead(this->_readyPin) == HIGH)
          {
          }
          this->startTransmission();
          this->read(CO_NATES, &readBuffer[0], readBufferLength); // Get Wheel Coordinates 0x02H

          Serial.println("Wheel Value: ");
          for (unsigned char i = 0; i < 3 ; i++) {
            Serial.print(readBuffer[i]); Serial.print(", ");
          }
        }
        mode = IQS263_MODE_CONFIGURED;
        break;
    }
  }
}


bool IQS263_TouchSensor::isFlickedRight() {
  unsigned char bufferLength = 2;
  unsigned char buffer[bufferLength];

  bool flickedRight = false;

  this->read(IQS263_TouchSensor::SYSTEM_FLAGS, &buffer[0], bufferLength);

  if (buffer[1] == 255) { 
    return false;
  }

  unsigned char reading = buffer[1];
  // we want to read bit 8
  reading = reading >> 7;
  reading = reading & 0x1;


  if (reading) {
    Serial.println("Flicked Right");
    flickedRight = true;
  }


  return flickedRight;
}


bool IQS263_TouchSensor::isFlickedLeft() {
  unsigned char bufferLength = 2;
  unsigned char buffer[bufferLength];

  bool flickedLeft = false;

  this->read(IQS263_TouchSensor::SYSTEM_FLAGS, &buffer[0], bufferLength);

  if (buffer[1] == 255) { 
    return false;
  }

  unsigned char reading = buffer[1];

  // we want to read bit 7
  reading = reading >> 6;
  reading = reading & 0x1;


  if (reading) {
    Serial.println("Flicked Left");
    flickedLeft = true;
  }

  return flickedLeft;
}




bool IQS263_TouchSensor::isTapped() {
  unsigned char bufferLength = 2;
  unsigned char buffer[bufferLength];

  bool tapped = false;

  this->read(IQS263_TouchSensor::SYSTEM_FLAGS, &buffer[0], bufferLength);


  if (buffer[1] == 255) { 
    return false;
  }

  unsigned char reading = buffer[1];

  // we want to read bit 6
  reading = reading >> 5;  
  reading = reading & 0x01;


  if (reading) {
    Serial.println("Tapped");
    tapped = true;
  }

  return tapped;
}

unsigned int IQS263_TouchSensor::getPosition() {
  unsigned char bufferLength = 3;
  unsigned char buffer[bufferLength];
  this->read(CO_NATES,  &buffer[0], bufferLength);
  Serial.print(buffer[0],BIN);
  Serial.print("\t");
  Serial.println(buffer[0]);
  return buffer[0];
}


bool IQS263_TouchSensor::isTouched() {
  unsigned char bufferLength = 2;
  unsigned char buffer[bufferLength];
  bool isTouched = false;
  this->read(TOUCH_STAT,  &buffer[0], bufferLength);

  unsigned char value = buffer[0];

  Serial.print(buffer[0], BIN);
  Serial.print("\t");
  Serial.print(buffer[0]);

  if (value > IQS263_TOUCH_THRESHOLD) {
    Serial.print("\t");
    Serial.print("touched");
    isTouched = true;
  }
  Serial.println(" ");
  return isTouched;
}


void IQS263_TouchSensor::startTransmission() {
  Wire.begin();        // join i2c bus (address optional for master)
  delay(5);

  //val = digitalRead(this->_readyPin );
  while (digitalRead(this->_readyPin) == 1)
  {
    //digitalWrite(this->_dataPin, HIGH);
    __asm__("nop\n\t");
  }
  digitalWrite(this->_dataPin, LOW);
  Wire.beginTransmission(this->_address);
  //Wire.endTransmission();
  //delay(1);
}


void IQS263_TouchSensor::stopTransmission() {
  int ret = Wire.endTransmission();
  //Serial.print("this->stopTransmission returned ");Serial.println(ret);
}

void IQS263_TouchSensor::repeat() {
  int ret = Wire.endTransmission(false);
}

void IQS263_TouchSensor::write(unsigned char write_addr, unsigned char *data, unsigned char NoOfBytes) {
  unsigned char i;
  Wire.write(write_addr);
  for (i = 0 ; i < NoOfBytes ; i++)
  {
    Wire.write(data[i]);

  }
  this->stopTransmission();
  delay(1);
}

void IQS263_TouchSensor::read(unsigned char read_addr, unsigned char *data, unsigned char NoOfBytes) {
  this->startTransmission();
  unsigned char i = 0;
  this->sendByte(read_addr);							// IQSthis->_dataPinxx address-command
  this->repeat(); //TODO: see into: this->repeat_start();
  Wire.requestFrom((uint8_t)this->_address, (uint8_t)NoOfBytes);
  while (Wire.available())   // slave may send less than requested
  {
    data[i] = Wire.read(); // receive a byte as character
    i++;
  }
}



unsigned char IQS263_TouchSensor::sendByte(unsigned char c) {
  Wire.write(c);
}