/**
 * This program takes 3 keys
 * 1) a master key
 * 2) an authorized key
 * 3) an unauthorized key
 * 
 * An unauthorized key can be authorized by using the master key 
 * to enter authorization mode, then using an unauthorized key
 * Any unauthorized key waived in authorization mode will be authorized
 * 
 * This program can store up to maxTags tags (25 at the time of this writing)
 * 
 * When a new key is added, it is stored ot EEPROM so that the key is not lost during power outage
 * 
 * When authorized key is used, a 5 second unlock is triggered. 
 * 
 * All key usages time out after 1 second.
 * 
 * ------------------------------------------
 * 
 * Connect RFID Reader TX to pin 3 and RFID Reader RX to pin 2
 * Connect Servo to pin 9
 * red led = pin 11
 * green led = pin 12
 * blue led = pin 13
 * 
 * ------------------------------------------
 * 
 * Interesting features to add:
 * - deauthorize feature, possibly by using master card and then authorized key to deauthorize
 */
#include <SoftwareSerial.h>
#include <Servo.h>
#include "CurieEEPROM.h"

const boolean reinitialize = false; // clear the EEPROM of all known tags

// Choose two pins for SoftwareSerial
const int tagReaderRxPin = 2;
const int tagReaderTxPin = 3;
SoftwareSerial rfTagReader(tagReaderRxPin, tagReaderTxPin); // RX, TX


// Pick a PWM pin to put the servo on
const int servoPin = 9;

// Make a servo object
Servo lockServo;


const int tagReaderBufferLength = 16;
const int idLength = 13;
const int numTags = 4;
const int etagStart = 2;
const int etagEnd = 3;
const int etagCarriageReturn = 10;
const int etagLineFeed = 13;

boolean nextTagSetsAsAuthorized = false;

char masterTag[idLength] = "82003C9698B0";
//char authorizedTag[idLength] = ""; //"82003CB2A3AF";

const int maxTags = 25;
int totalTags = 0;
char authorizedTags[maxTags][idLength];

boolean tagRead = false;
unsigned long tagReadClearTime = 1000; // 2 second clear time
unsigned long lastReadTime;

boolean locked = true;
unsigned long doorUnlockClearTime = 5000;
unsigned long lastUnlockTime;


// Empty array to hold a freshly scanned tag
char incomingTag[idLength];
char lastTag[idLength];


void saveAuthorizedTag(char* tag);
void setLock(boolean);
boolean tagsMatch(char tag1[], char tag2[]);


// indicator lights
const int unauthorizedTagLedPin = 11;
const int authorizedTagLedPin = 12;
const int masterTagLedPin = 13;

void setup() {
  // Starts the hardware and software serial ports
  Serial.begin(9600);
  while (!Serial);
  Serial.println("DOOR LOCK STARTED");
  rfTagReader.begin(9600);


  // reset memory if necessary
  if (reinitialize) {
    int length = maxTags * idLength;
    for (int address = 0; address < length; address++) {
     EEPROM.write(address, 0); 
    }
  }
  
  pinMode(unauthorizedTagLedPin, OUTPUT);
  pinMode(authorizedTagLedPin, OUTPUT);
  pinMode(masterTagLedPin, OUTPUT);
  
  // Attaches the servo to the pin
  lockServo.attach(servoPin);
  
  // Put servo in locked position
  lockServo.write(0);
  
  lastReadTime = millis();
  lastUnlockTime = millis();
  
  // loads keys into memory
  loadTagsFromEEPROM();

  // print authorized tags
  printTagMap();
  
  setLock(true); // lock door
}

void loop() {
  unsigned long currentTime = millis();
  
  if (locked == false) {
    if (currentTime > (lastUnlockTime + doorUnlockClearTime)) {
      setLock(true);
    }
  }


  
  // wait between scans
  if (tagRead) {
    if (currentTime < (lastReadTime + tagReadClearTime)) {
      return;
    } else {
      Serial.println("CLEARED");
      digitalWrite(unauthorizedTagLedPin, LOW);
      digitalWrite(authorizedTagLedPin, LOW);
      digitalWrite(masterTagLedPin, LOW);
      lastReadTime = currentTime;
      tagRead = false;
    }
  }
  
  // Counter for the incomingTag array
  int i = 0;
  // Variable to hold each byte read from the serial buffer
  int readByte;
  // Flag so we know when a tag is over
  boolean finishedReadingTag = false;

  // This makes sure the whole tag is in the serial buffer before
  // reading, the Arduino can read faster than the ID module can deliver!
  if (rfTagReader.available() == tagReaderBufferLength) {
    finishedReadingTag = true;
  }

  if (finishedReadingTag == true) {
    while (rfTagReader.available()) {
      // Take each byte out of the serial buffer, one at a time
      readByte = rfTagReader.read();


      /* This will skip the first byte (2, STX, start of text) and the last three,
      ASCII 13, CR/carriage return, ASCII 10, LF/linefeed, and ASCII 3, ETX/end of 
      text, leaving only the unique part of the tag string. It puts the byte into
      the first space in the array, then steps ahead one spot */
      if (readByte != etagStart && readByte!= etagLineFeed && readByte != etagCarriageReturn && readByte != etagEnd) {
        incomingTag[i] = readByte;
        i++;
      }

      // If we see ASCII 3, ETX, the tag is over
      if (readByte == etagEnd) {
        finishedReadingTag = false;
      }

    }
    Serial.print("incoming tag: ");
    Serial.println(incomingTag);
  }



  // don't do anything if the incomingTag array is full of zeroes
  if (strlen(incomingTag) == 0) {
    //Serial.print(".");
    return;
  } else {
    tagRead = true;
  }

  // save this tag as an authorized tag
  if (nextTagSetsAsAuthorized) {
    saveAuthorizedTag(incomingTag);
    nextTagSetsAsAuthorized = false;

    
    Serial.println("Writing new authorized tag: ");
    Serial.print(incomingTag);
    Serial.println();

    printTagMap();
  } 
  // master tag scanned.  Next tag will be an authorized tag
  else if (tagsMatch(incomingTag, masterTag)) {
    nextTagSetsAsAuthorized = true;

    
    Serial.print("Master card read: ");
    Serial.println(incomingTag);
    Serial.println("Next card will be an authorized tag");
    Serial.println();
    
    digitalWrite(masterTagLedPin, HIGH);
  }
  // If incomingTag matched any of the tags
  // we checked against, total will be 1
  boolean matchFound = false;
  for (int k=0; k < maxTags; k++) {
    if (tagsMatch(incomingTag, authorizedTags[k])) {
    
      // Put the action of your choice here!
    
      // I'm going to rotate the servo to symbolize unlocking the lockbox
      matchFound = true;
      break;
    }
  }

  if (matchFound) {
    Serial.print("Read unlock tag: ");
    Serial.println(incomingTag);
    digitalWrite(authorizedTagLedPin, HIGH);
    setLock(false);
  
  }

  else {
    // prevent printing out messages when we've change the system state
    if (!nextTagSetsAsAuthorized) {
      // This prints out unknown tags so you can add them to your authorizedTags as needed
      Serial.print("Unknown tag: ");
      Serial.println(incomingTag);
      //printTag();
      digitalWrite(unauthorizedTagLedPin, HIGH);
    }
  }


  // Once incomingTag has been checked, fill it with zeroes
  // to get ready for the next tag read
  for (int index=0; index < idLength; index++) {
    incomingTag[index] = 0;
  }
}


void loadTagsFromEEPROM() {
  // zero out tags
  for (int k=0; k<maxTags; k++) {
    for (int c=0; c < idLength; c++) {
      authorizedTags[k][c] = '\0';
    }
  }
  
  // read from eeprom
  for (int k=0; k<totalTags; k++) {
    readStringFromProgmem(k, idLength, authorizedTags[k]);
  }
  
}

// save a tag as an authorizedTag 
void saveAuthorizedTag(char tag[]) {
  totalTags = totalTags+1;
  if (totalTags > maxTags) {
    totalTags = 0;
  }
  
  for (int c = 0; c < idLength; c++) {
    authorizedTags[totalTags][c] = tag[c];
  }

  writeStringToProgmem(totalTags, idLength, tag);
  
}

// lock or unlock the door
void setLock(boolean setLocked) {
  if (setLocked) {
    Serial.println("LOCKING DOOR");
    lockServo.write(0);
  } else {
    Serial.println("UNLOCKING DOOR");
    lockServo.write(180);
    lastUnlockTime = millis();
  }
  locked = setLocked;
}

// This function steps through both newTag and one of the known
// tags. If there is a mismatch anywhere in the tag, it will return 0,
// but if every character in the tag is the same, it returns 1
boolean tagsMatch(char tag1[], char tag2[]) {
    for (int i = 0; i < idLength; i++) {
      if (tag1[i] != tag2[i]) {
        return false;
      }
    }
  return true;
}


/*
void printTag(int* tag) {
  for (int i=0; i<idLength; i++) {
    Serial.print(tag[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");  
}
*/

void readStringFromProgmem(int offset, int stringLength, char* buffer) {
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

void printTagMap() {
  Serial.print(totalTags);
  Serial.println(" authorized tags found:");
  for (int k=0; k < maxTags; k++) {
    if ((authorizedTags[k][0] >= 48) && (authorizedTags[k][0] <= 90)) {
      Serial.print(" - ");
      Serial.println(authorizedTags[k]);
    }
  }
}

