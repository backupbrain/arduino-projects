/**
 * This rpogram generates a couple const PROGMEM strings that represent
 * pixel struct data as a series of unsigned longs.
 * 
 * It is designed to take the output as text so that you can copy and paste it into 
 * a program that animates neopixels colors by pulling pixel values from progmem
 */
#include "FireplacePixel.h"

#define MIN 0
#define MAX 1
#define SMALL 0
#define MEDIUM 1
#define LARGE 2


// {MIN, MAX} values for color
const byte h[2] = {0, 25}; // 35 is the theoretical max but looks a bit green to me
const byte s[2] = {190, 255};
const byte v[2] = {0, 75};

unsigned int timeouts[3] = {704, 1056, 1304}; // divisible by 8

const unsigned int NUM_PIXEL_VALUES = (64); 

Pixel pixels[NUM_PIXEL_VALUES];


void setup() {
  Serial.begin(9600);
  while (!Serial);


  boolean h_val = MIN;
  boolean s_val = MIN;
  boolean v_val = MIN;
  
  unsigned int timeout_val = SMALL;
  
  byte loop_max = NUM_PIXEL_VALUES; //(pow(2, 6)-3)*3;
 
  unsigned int timeout = SMALL;
  unsigned int index = 0;
  unsigned int boolean_mask = 0b0;
  unsigned int working_value;
  boolean done = false;
   
  hsv colorTransition; 
  
  while (!done) {
    Serial.println("-------------------");
    boolean_mask++;
    
    Serial.print("index: ");
    Serial.println(index);
    
    Serial.println(boolean_mask, BIN);
    
    int format = BIN;
    
    working_value = boolean_mask;    
    
      pixels[index].color.h = rand() % (h[1]-h[0]) + h[0];
      pixels[index].color.s = rand() % (s[1]-s[0]) + s[0];
      pixels[index].color.v = rand() % (v[1]-v[0]) + v[0];
      
      timeout = timeouts[rand() % 3];
      pixels[index].timeout = timeouts[timeout];
      
      
      Serial.println(timeouts[timeout], DEC);
      
      index++;
      if (index >= loop_max) {
        done = true;
      }
    
    
     
  }
  
  
  for (unsigned int i=0; i<loop_max; i++) {
    printPixel(pixels[i]);
  }
  
  //unsigned long* packedPixel;
  
  boolean h_switched = false;
  
  Serial.print("const byte numPixels = ");
  Serial.print(NUM_PIXEL_VALUES);
  Serial.println(";");
  
  Serial.println("const unsigned long pixels[] PROGMEM = {");
  for (unsigned int i=0; i<loop_max; i++) {
   
        unsigned long packedPixel = packPixel(pixels[i]);
        /*
        Serial.print(i);
        Serial.print(": ");
        Serial.print(packedPixel[0], BIN);
        Serial.print(", ");
        Serial.print(packedPixel[1], BIN);
        Serial.println(",");
        /* */
        
         
  }
  Serial.println("};");

}


void loop() {


}

Pixel unpackPixel(unsigned int package) {
  Pixel pixel;

  // pixel package length shuould be 40 bits

  // first three bytes are hsv
  pixel.timeout = package  & 0xF;
  pixel.color.v = package >> 4 & 0xF;
  pixel.color.s = package >> 4 & 0xF;
  pixel.color.h = package >> 4 & 0xF;

  return pixel;
}

/*
unsigned long* packPixel(Pixel pixel) {
  unsigned long package1;
  unsigned long package2;

  Serial.println();
  Serial.println("Bit Math:");
  Serial.println("==================");

  package1 = pixel.fromColor.h;  // 8 bits
  Serial.println(package1, BIN);
  package1 <<= 8;
  package1 |= pixel.fromColor.s; // 16 bits
  Serial.println(package1, BIN);
  package1 <<= 8;
  package1 |= pixel.fromColor.v; // 24 bits
  Serial.println(package1, BIN);
  package1 <<= 8;                 
  package1 |= pixel.toColor.h;  // 32 bits
  Serial.println(package1, BIN);

  package2 = pixel.toColor.s; // 8 bits
  Serial.print(package1, BIN);
  Serial.print(" ");
  Serial.println(package2, BIN);
  package2 <<= 8;
  package2 |= pixel.toColor.v; // 16 bits
  Serial.print(package1, BIN);
  Serial.print(" ");
  Serial.println(package2, BIN);
  package2 <<= 16;          
  package2 |= pixel.timeout;  // 32 bits
  Serial.print(package1, BIN);
  Serial.print(" ");
  Serial.println(package2, BIN);

  Serial.println();
  Serial.println("Pixel:");
  Serial.println("==================");


  Serial.print(package1, DEC);
  Serial.print(" ");
  Serial.print(package2, DEC);
  Serial.println();

  Serial.print(package1, HEX);
  Serial.print(" ");
  Serial.print(package2, HEX);
  Serial.println();

  Serial.print(package1, BIN);
  Serial.print(" ");
  Serial.print(package2, BIN);
  Serial.println();
  
  
  unsigned long package[2] = {package1, package2};
  
  return package;
}
/* */



void printPixel(Pixel pixel) {
  int format = DEC;
  Serial.println();
  Serial.println("Pixel: ");
  
  Serial.print("from h: ");
  Serial.println(pixel.color.h, format);
  Serial.print("from s: ");
  Serial.println(pixel.color.s, format);
  Serial.print("from v: ");
  Serial.println(pixel.color.v, format);
  
  Serial.print("timeout: ");
  Serial.println(pixel.timeout, format);
}

unsigned long packPixel(Pixel pixel) {
  unsigned long package;

  
  byte timeout = pixel.timeout/8;

  package = pixel.color.h;  // 8 bits
  package <<= 8;
  package |= pixel.color.s; // 16 bits
  package <<= 8;
  package |= pixel.color.v; // 24 bits
  package <<= 8;                 
  package |= timeout;  // 32 bits
  
  Serial.print("  0x");
  Serial.print(package, HEX);
  Serial.println(",");
  
  return package;
}
/* */
