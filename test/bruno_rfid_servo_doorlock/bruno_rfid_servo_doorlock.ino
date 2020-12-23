
#include <SoftwareSerial.h>
#include <Servo.h>

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



char authorizedTags[numTags][idLength] = {
             "82003CB2A3AF",
             "444444444444",
             "555555555555",
             "7A005B0FF8D6"
};


// Empty array to hold a freshly scanned tag
char incomingTag[idLength];


void setup() {
  // Starts the hardware and software serial ports
   Serial.begin(9600);
   rfTagReader.begin(9600);

   // Attaches the servo to the pin
   lockServo.attach(servoPin);

   // Put servo in locked position
   lockServo.write(0);
}

void loop() {
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
  }


  // don't do anything if the incomingTag array is full of zeroes
  if (strlen(incomingTag)== 0) {
    return;
  }

  else {
    boolean matchFound = false;

    for (int tagIndex=0; tagIndex < numTags; tagIndex++){
        if (tagsMatch(incomingTag, authorizedTags[tagIndex])) {
          matchFound = true;
        }
    }

    // If incomingTag matched any of the tags
    // we checked against, total will be 1
    if (matchFound) {

      // Put the action of your choice here!

      // I'm going to rotate the servo to symbolize unlocking the lockbox

      Serial.println("Success!");
      lockServo.write(180);
    }

    else {
        // This prints out unknown tags so you can add them to your authorizedTags as needed
        Serial.print("Unknown tag! ");
        Serial.print(incomingTag);
        Serial.println();
    }
  }

  // Once incomingTag has been checked, fill it with zeroes
  // to get ready for the next tag read
  for (int index=0; index < idLength; index++) {
    incomingTag[index] = 0;
  }
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
