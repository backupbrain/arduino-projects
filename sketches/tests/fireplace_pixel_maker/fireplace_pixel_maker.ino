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
#define FROM 0
#define TO 1
#define SMALL 0
#define MEDIUM 1
#define LARGE 2


// {MIN, MAX} values for color
const byte h[2] = {0, 35};
const byte s[2] = {190, 255};
const byte v[2] = {0, 75};

unsigned int timeouts[3] = {700, 1050, 1300};

const unsigned int NUM_PIXEL_VALUES = (64-3); //*3;


Pixel pixels[NUM_PIXEL_VALUES];


void setup() {
  Serial.begin(9600);
  while (!Serial);


  boolean from_h_val = MIN;
  boolean from_s_val = MIN;
  boolean from_v_val = MIN;
  
  boolean to_h_val = MAX;
  boolean to_s_val = MIN;
  boolean to_v_val = MIN;
  
  unsigned int timeout_val = SMALL;
  
  byte loop_max = NUM_PIXEL_VALUES; //(pow(2, 6)-3)*3;
 
  unsigned int timeout = SMALL;
  unsigned int index = 0;
  unsigned int boolean_mask = 0b0;
  unsigned int working_value;
  boolean done = false;
   
  hsv colorTransition[2]; 
  
  while (!done) {
    Serial.println("-------------------");
    boolean_mask++;
    
    Serial.print("index: ");
    Serial.println(index);
    
    Serial.println(boolean_mask, BIN);
    
    int format = BIN;
    
    working_value = boolean_mask;
    Serial.println(v[working_value & 0b1], format);
    colorTransition[TO].v = v[working_value & 0b1];
    working_value >>= 1;
    Serial.println(s[working_value & 0b1], format);
    colorTransition[TO].s = s[working_value & 0b1];
    working_value >>= 1;
    Serial.println(h[working_value] & 0b1, format);
    colorTransition[TO].h = h[working_value & 0b1];
    
    
    working_value >>= 1;
    Serial.println(v[working_value & 0b1], format);
    colorTransition[FROM].v = v[working_value & 0b1];
    working_value >>= 1;
    Serial.println(s[working_value & 0b1], format);
    colorTransition[FROM].s = s[working_value & 0b1];
    working_value >>= 1;
    Serial.println(h[working_value & 0b1], format);
    colorTransition[FROM].h = h[working_value & 0b1];
    
    //for (timeout = 0; timeout < 3; timeout++) {
      pixels[index].fromColor.h = colorTransition[FROM].h;
      pixels[index].fromColor.s = colorTransition[FROM].s;
      pixels[index].fromColor.v = colorTransition[FROM].v;
      pixels[index].toColor.h = colorTransition[TO].h;
      pixels[index].toColor.s = colorTransition[TO].s;
      pixels[index].toColor.v = colorTransition[TO].v;
      
      timeout = timeouts[rand() % 3];
      pixels[index].timeout = timeouts[timeout];
      
      
      Serial.println(timeouts[timeout], DEC);
      
      
      // only move to the next pixel if there is some value that's different
      if (
        (pixels[index].fromColor.h != pixels[index].toColor.h) ||
        (pixels[index].fromColor.s != pixels[index].toColor.s) ||
        (pixels[index].fromColor.v != pixels[index].toColor.v)) {
         
          
        index++;
      }
        
      if (index >= loop_max) {
        done = true;
      }
    //}
    
    
     
  }
  
  
  for (unsigned int i=0; i<loop_max; i++) {
    printPixel(pixels[i]);
  }
  
  //unsigned long* packedPixel;
  
  boolean h_switched = false;
  
  Serial.print("const unsigned byte numPixels = ");
  Serial.print(NUM_PIXEL_VALUES);
  Serial.println(";");
  
  Serial.println("const unsigned long pixels_from_red[] PROGMEM = {");
  for (unsigned int i=0; i<loop_max; i++) {
   
        unsigned long* packedPixel = packPixel(pixels[i]);
        /*
        Serial.print(i);
        Serial.print(": ");
        Serial.print(packedPixel[0], BIN);
        Serial.print(", ");
        Serial.print(packedPixel[1], BIN);
        Serial.println(",");
        /* */
        
        if (!h_switched && (pixels[i].fromColor.h == h[MAX])) {
          h_switched = true;
          Serial.println("}; ");
          Serial.println("const unsigned long pixels_from_yellow[] PROGMEM = {");
        }
         
  }
  Serial.println("};");

}


void loop() {


}

Pixel unpackPixel(unsigned int timeout, unsigned int package1, unsigned int package2) {
  Pixel pixel;

  // pixel package length shuould be 40 bits

  // first three bytes are hsv
  pixel.fromColor.v = package1 & 0xF;
  pixel.fromColor.s = package1 >> 4 & 0xF;
  pixel.fromColor.h = package1 >> 4 & 0xF;

  pixel.toColor.v = package2 & 0xF;
  pixel.toColor.s = package2 >> 4 & 0xF;
  pixel.toColor.h = package2 >> 4 & 0xF;

  pixel.timeout = timeout;

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
  Serial.println(pixel.fromColor.h, format);
  Serial.print("from s: ");
  Serial.println(pixel.fromColor.s, format);
  Serial.print("from v: ");
  Serial.println(pixel.fromColor.v, format);
  
  Serial.print("to h: ");
  Serial.println(pixel.toColor.h, format);
  Serial.print("to s: ");
  Serial.println(pixel.toColor.s, format);
  Serial.print("to v: ");
  Serial.println(pixel.toColor.v, format);
  
  Serial.print("timeout: ");
  Serial.println(pixel.timeout, format);
}

unsigned long* packPixel(Pixel pixel) {
  unsigned long package1;
  unsigned long package2;


  package1 = pixel.fromColor.h;  // 8 bits
  package1 <<= 8;
  package1 |= pixel.fromColor.s; // 16 bits
  package1 <<= 8;
  package1 |= pixel.fromColor.v; // 24 bits
  package1 <<= 8;                 
  package1 |= pixel.toColor.h;  // 32 bits

  package2 = pixel.toColor.s; // 8 bits
  package2 <<= 8;
  package2 |= pixel.toColor.v; // 16 bits
  package2 <<= 16;          
  package2 |= pixel.timeout;  // 32 bits
  
  Serial.print("  0x");
  Serial.print(package1, HEX);
  Serial.print(", 0x");
  Serial.print(package2, HEX);
  Serial.println(",");
  
  unsigned long package[2] = {package1, package2};
  
  return package;
}
/* */
