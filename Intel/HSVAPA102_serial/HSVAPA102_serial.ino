#include <Adafruit_NeoPixel.h>

#include "HSVAPA102.h"
#include "ColorConverter.h"

/**
 * Serial Communication
 */
const unsigned int serialBaudRate = 115200;

/**
 * LED Configuration
 */
const unsigned int neoPixelPin = 11;
const unsigned int numPixels = 16;


/******************************************
 * 
 * Do not modify anything under this line
 * 
 ******************************************/

const unsigned char FRAME_SEGMENT_GAMMA = 0;
const unsigned char FRAME_SEGMENT_HUE = 1;
const unsigned char FRAME_SEGMENT_SATURATION = 2;
const unsigned char FRAME_SEGMENT_VALUE = 3; 


Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

#define ARRAYSIZE(X) (sizeof(X)/sizeof(*X))

unsigned char frameBuffer[numPixels*4+12];

const int bufferSize = 16;
const int numFrames = 4;
//unsigned char frameBuffer[bufferSize][numFrames];
unsigned char incomingByte;
unsigned char header;
int zeroCounter = 0;
boolean syncingData = true;
int arrayIndex = 0;

unsigned int pixel = 0;


void setup() {
  Serial.begin(serialBaudRate);
   
  strip.begin();
  
  for (int pixel=0; pixel<strip.numPixels(); pixel++) {
    strip.setPixelColor(pixel, strip.Color(10, 10, 10));
  }
  strip.show();
}

/*
static const int NBuffer = (16+1)*4;
static byte byteBuffer[NBuffer];
static int byteBufferSize = 0;
void PushBuffer(char d) {
  // If the buffer is full then shift all elements down by 1.
  if (byteBufferSize == NBuffer) {
    for (int i = 0; i < NBuffer - 1; ++i) {
      byteBuffer[i] = byteBuffer[i+1];
    }
  }
  
  byteBuffer[byteBufferSize];
  
  else {
    // Otherwise increment the pointer.
    byteBufferSize++;
  }

  // byteBufferPos should now be validated as an index
  // to write into.
  byteBuffer[byteBufferPos] = d;
}

void ClearBuffer() {
  byteBufferPos = 0;
}

bool BufferHasFourTrailingZeros() {
  if (byteBufferPos)
}
*/

boolean redraw = false;
void loop()  {

  
  //Serial.println("Aaaaaarh");

  /*
        for (int i=0; i<strip.numPixels(); i++) {
          int randNumber = random(32);
          strip.setPixelColor(i, randNumber, randNumber, randNumber);
        }
        strip.show();
        /* */
        
  // procedure: read serial one bit at a time and fill buffer
  // when we see a 0x0000 0x0000 0x0000 0x000, draw the frame

  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
  
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);

    // look for zeroes at beginning of frame
    if (incomingByte == 0x00) {
      zeroCounter++;
      syncingData = false;
    } else {
      zeroCounter = 0;
    }

    // if we get 4 consecutive 0x00, prepare to redraw the framebuffer
    if (zeroCounter >= 4) {
      arrayIndex = 0;
      zeroCounter = 0;
      redraw = true;
    }

    // if we aren't waiting for our first packet of data, then we should
    // start loading data into the buffer
    if (!syncingData) {
      frameBuffer[arrayIndex] = incomingByte;

      // Only write so far as the end of the array.
      arrayIndex = max(ARRAYSIZE(frameBuffer) - 1, arrayIndex + 1);
    }

    // finally, when it comes time to redraw the framebuffer,
    // we should output the new pixels
    if (redraw) {
      
      Serial.print("npixels: "); Serial.println(strip.numPixels());
      
      for (int i=0; i<strip.numPixels(); i++) {
        int r = random(32);
        strip.setPixelColor(i, r, r, r);
        //int p = i*4+3;
        //strip.setPixelColor(i, frameBuffer[p], frameBuffer[p], frameBuffer[p]);
      }
      strip.show();
    }
    
          
  }
  /*
  while (Serial.available()) {
      var = Serial.read();

      // look out for zeroes!
      if (var == 0x0000) {
        zeroCounter++;
        syncingData = false;
      } else {
        zeroCounter = 0;
      }

      // reset the buffer when we get to the fourth zero, mark frame as dirty
      if (zeroCounter >= 4) {
        arrayIndex = 0;
        zeroCounter = 0;
        redraw = true;
      }

      // if we aren't syncing data (which happens during the first loop)
      if (!syncingData) {
        frameBuffer[arrayIndex] = var;
        
      }

      // if the frame needs to be redrawn, do so
      if (redraw) {
        for (int i=0; i<strip.numPixels(); i++) {
          arrayIndex = i*4+3;
          int randNumber = random(32);
          //strip.setPixelColor(i, frameBuffer[arrayIndex], frameBuffer[arrayIndex], frameBuffer[arrayIndex]);
          strip.setPixelColor(i, randNumber, randNumber, randNumber);
        }
        strip.show();
        redraw = false;
      }
      
    
  }
  /* */
}
    
