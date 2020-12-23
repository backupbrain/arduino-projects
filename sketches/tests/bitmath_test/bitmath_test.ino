#include "bitmath.h"

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("Size of long: ");
  Serial.println(sizeof(unsigned long));

  unsigned long maxlong = 0b11111111111111111111111111111111;
  Serial.println(maxlong, DEC);
  Serial.println(maxlong, HEX);
  Serial.println(maxlong, BIN);
  Serial.println("");

  Pixel pixelIn;
  pixelIn.fromColor.h = 255;
  pixelIn.fromColor.s = 128;
  pixelIn.fromColor.v = 96;


  pixelIn.toColor.h = 64;
  pixelIn.toColor.s = 32;
  pixelIn.toColor.v = 16;

  pixelIn.timeout = 1500;
  
  packPixel(pixelIn);
  
  Pixel pixel2;
  pixel2.fromColor.h = 0;
  pixel2.fromColor.s = 255;
  pixel2.fromColor.v = 128;


  pixel2.toColor.h = 15;
  pixel2.toColor.s = 230;
  pixel2.toColor.v = 96;

  pixel2.timeout = 500;

  packPixel(pixel2);
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

void packPixel(Pixel pixel) {
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
}

