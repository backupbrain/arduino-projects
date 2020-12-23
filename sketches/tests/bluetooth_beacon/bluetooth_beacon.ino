/**
 * bluetooth_beacon - outputs a device name indefinitely over Bluetooth Low Energy
 *
 * @author Tony Gaitatzis 2015-07
 */
#include <SPI.h>
#include "Adafruit_BLE_UART.h"


////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE CONSTANTS
// Change these to change how the program records and processes data
////////////////////////////////////////////////////////////////////////////////

/**
 * Beacon configuration
 */
char beaconName[] = "ADONIS"; // 7 chars max!
const unsigned int animationFrameTimeout_ms = 3000;


/**
 * System configuration
 */
#define POWER_LED_PIN 13
const boolean powerIndicatorOn = true;

/**
 * Bluetooth Breakout configuration
 */
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST 9



////////////////////////////////////////////////////////////////////////////////
// INTERNAL STATE
// These shouldn't be modified unless you know what you're doing.
////////////////////////////////////////////////////////////////////////////////

unsigned long lastAnimationFrameUpdateTime;
Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);


void setup(void) { 
  Serial.begin(9600);
  while(!Serial); // Leonardo/Micro should wait for serial init
  Serial.println(F("Beacon Test"));

  BTLEserial.setDeviceName(beaconName); /* 7 characters max! */
  BTLEserial.begin();
  
  if (powerIndicatorOn) {
    pinMode(POWER_LED_PIN, OUTPUT);
    digitalWrite(POWER_LED_PIN, HIGH);
  }
}


void loop() {
  BTLEserial.pollACI();
}
