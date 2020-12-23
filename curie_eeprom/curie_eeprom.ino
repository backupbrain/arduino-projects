#include "CurieEEPROM.h"

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Running EEPROM Test for Curie/101");
  Serial.print("EEPROM Size: ");
  Serial.print(EEPROM.length());
  Serial.println(" (NOTE: Should be 1024)");
  write_eeprom();
  dump_eeprom();
}

void loop() {
  delay(1000);
}

void print_byte(byte value) {
  if (value <= 0x0F) {
    Serial.print(" 0");
  } else {
    Serial.print(' ');
  }
  Serial.print(value, HEX);
}

void write_eeprom() {
  int len = 15;
  byte buf[len];
  for (int i = 0; i < len; i++) {
    buf[i] = i + 1;
  }
  Serial.print("Writing ");
  Serial.print(len);
  Serial.println(" bytes");
  for (int i = 0; i < len; i++) {
    byte value = buf[i];
    EEPROM.update(i, value);
    if (value == EEPROM.read(i)) {
      print_byte(value);
    } else {
      Serial.println("\nWRITE FAILED");
      return;
    }
  }
  Serial.println("\nSUCCESS?");
}

void dump_eeprom() {
  Serial.println("Dump EEPROM");
  for (int i = 0; i < EEPROM.length(); i++) {
    int value = EEPROM.read(i);
    print_byte(value);
    if (i > 0 && ((i + 1) % 32 == 0)) {
      Serial.println();
    }
  }
}



