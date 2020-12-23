static uint8_t pirPin = 1;

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int pirCalibrationTime_s = 30;    
int pirCooldownTime_ms = 10;

static uint8_t PIR_STATE_MOTION_SENSED = 0x01;
static uint8_t PIR_STATE_NO_MOTION_SENSED = 0x02;
uint8_t pirState;
boolean isPirStateChanged = false;
boolean isPirInitialized = true;

void setupPir() {
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);

  pirState = PIR_STATE_NO_MOTION_SENSED;
  isPirStateChanged = false;

  
  //Serial.println("PIR Initializing...");

  if (!isPirInitialized) {
    delay(pirCalibrationTime_s * SECONDS);
    isPirInitialized = true;
  }

  //Serial.println("PIR Initialized");
}


void loopPir() {
  
  int pirReading = digitalRead(pirPin);
  int newPirState;
  if (pirReading == HIGH) { 
    newPirState = PIR_STATE_MOTION_SENSED;
    if (pirState == PIR_STATE_NO_MOTION_SENSED) {
      isPirStateChanged = true;
    } else {
      isPirStateChanged = false;
    }
  } else {
    newPirState = PIR_STATE_NO_MOTION_SENSED;
    if (pirState == PIR_STATE_MOTION_SENSED) {
      isPirStateChanged = true;
    } else {
      isPirStateChanged = false;
    }
  }
  pirState = newPirState;

  //delay(pirCooldownTime_ms);
  
}

/* */

