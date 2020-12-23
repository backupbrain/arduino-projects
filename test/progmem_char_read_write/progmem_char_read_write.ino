#include "CurieEEPROM.h"

int stringLength = 13;

const char strings[] PROGMEM  = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};


void setup() {
  Serial.begin(9600);
  while (!Serial);

  
  // put your setup code here, to run once:
  // read back a 2-byte int

/*
  // read back a char
  int progmemLength = strlen_P(strings); // length of entire progmem
  int numStrings = progmemLength/stringLength;
  for (int s = 0; s < numStrings; s++) {
    for (int c = 0; c < stringLength; c++) {
      
      myChar =  pgm_read_byte_near(strings + (s*stringLength) + c);
      Serial.print(myChar);
    }
    Serial.println();
    
  }
  */

  int progmemLength = strlen_P(strings); // length of entire progmem
  int numStrings = progmemLength/stringLength;
  char buffer[stringLength+1];

  Serial.println("Reading:");
  Serial.println("===========");
  // print progmem again
  for (int s = 0; s < numStrings; s++) {
    readStringFromProgmem(s, stringLength, buffer);
  }


  Serial.println("Writing:");
  Serial.println("===========");
  // write new progmem
  char* newString = "123456789ABCD";
  writeStringToProgmem(0, stringLength, newString);


  Serial.println("Reading:");
  Serial.println("===========");
  // print progmem again  
  for (int s = 0; s < numStrings; s++) {
    readStringFromProgmem(s, stringLength, buffer);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

void readStringFromProgmem(int offset, int stringLength, char* buffer) {
  char myChar;
  for (int c = 0; c < stringLength; c++) {
    int address = (offset*stringLength) + c;
    buffer[c] = EEPROM.read(address);
  }
  buffer[stringLength] = '\0';
  Serial.println(buffer);  
}

void writeStringToProgmem(int offset, int stringLength, char* string) {
  for (int c = 0; c < stringLength; c++) {
    int address = (offset*stringLength)+c;
    EEPROM.write(address, (uint8_t)string[c]);
  }
  Serial.println();
}

