#include <avr/pgmspace.h>
#include "progmem_test.h"
#include <Adafruit_NeoPixel.h>



// save some unsigned ints
const uint16_t charSet[] PROGMEM = { 6500, 32796, 16843, 10, 11234};
const char signMessage[] PROGMEM = {"I AM PREDATOR, UNSEEN COMBATANT, CREATED BY THE UNITED STATES DEPART"};

const char string_0[] PROGMEM = "String 0";
const char string_1[] PROGMEM = "String 1";
const char string_2[] PROGMEM = "String 2";
const char string_3[] PROGMEM = "String 3";
const char string_4[] PROGMEM = "String 4"; 
const char string_5[] PROGMEM = "String 5";

const char* const string_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5};


const unsigned int NUM_PIXELS = 12;
const byte NEOPIXEL_PIN = 9;

const byte numPixelColors = 55;
const unsigned long pixels[] PROGMEM = {
  0xBE0000, 0xBE4BF41E,
  0xBE0000, 0xFF000505,
  0xBE0000, 0xFF4B0000,
  0xBE0023, 0xBE000000,
  0xBE0023, 0xBE4B0505,
  0xBE0023, 0xFF000505,
  0xBE0023, 0xFF4B0505,
  0xBE4B00, 0xBE000505,
  0xBE4B00, 0xFF000000,
  0xBE4B00, 0xFF4B0505,
  0xBE4B23, 0xBE00F41E,
  0xBE4B23, 0xBE4B0505,
  0xBE4B23, 0xFF000505,
  0xBE4B23, 0xFF4B0505,
  0xFF0000, 0xBE000000,
  0xFF0000, 0xBE4B0505,
  0xFF0000, 0xFF4B0000,
  0xFF0023, 0xBE00F41E,
  0xFF0023, 0xBE4B0000,
  0xFF0023, 0xFF000000,
  0xFF0023, 0xFF4BF41E,
  0xFF4B00, 0xBE00F41E,
  0xFF4B00, 0xBE4B0000,
  0xFF4B00, 0xFF000505,
  0xFF4B23, 0xBE000000,
  0xFF4B23, 0xBE4B0000,
  0xFF4B23, 0xFF000000,
  0xFF4B23, 0xFF4BF41E,
  
  0x23BE0000, 0xBE000000,
  0x23BE0000, 0xBE4B0000,
  0x23BE0000, 0xFF000000,
  0x23BE0000, 0xFF4BF41E,
  0x23BE0023, 0xBE4BF41E,
  0x23BE0023, 0xFF00F41E,
  0x23BE0023, 0xFF4BF41E,
  0x23BE4B00, 0xBE000505,
  0x23BE4B00, 0xBE4B0505,
  0x23BE4B00, 0xFF000505,
  0x23BE4B00, 0xFF4B0000,
  0x23BE4B23, 0xBE000000,
  0x23BE4B23, 0xFF00F41E,
  0x23BE4B23, 0xFF4B0505,
  0x23FF0000, 0xBE000000,
  0x23FF0000, 0xBE4BF41E,
  0x23FF0000, 0xFF000505,
  0x23FF0000, 0xFF4B0000,
  0x23FF0023, 0xBE000505,
  0x23FF0023, 0xBE4BF41E,
  0x23FF0023, 0xFF4BF41E,
  0x23FF4B00, 0xBE000000,
  0x23FF4B00, 0xBE4B0000,
  0x23FF4B00, 0xFF000000,
  0x23FF4B00, 0xFF4B0505,
  0x23FF4B23, 0xBE00F41E,
  0x23FF4B23, 0xBE4B0505,
  0x23FF4B23, 0xFF000505
  /*
  0xBE0000, 0xBE4B0000,
  0xBE0000, 0xFF00F41E,
  0xBE0000, 0xFF4B0000,
  0xBE0023, 0xBE000000,
  0xBE0023, 0xBE4B0000,
  /* */
};

PixelPointer pixelPointers[NUM_PIXELS];


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

unsigned int displayInt;
int k; //counter variable
char myChar;
unsigned long lastTime = 0;
unsigned long currentTime = 0;

char buffer[30];

void setup() {
  Serial.begin(9600);
  while (!Serial);
  /*
    // read back a 2-byte int
    for (k=0; k<5; k++) {
      displayInt = pgm_read_word_near(charSet+k);
      Serial.println(displayInt);
    }
    Serial.println();
    
    // read back a char
    for (k=0; k< strlen(signMessage); k++) {
      myChar = pgm_read_byte_near(signMessage+k);
      Serial.print(myChar);
    }
    Serial.println();
    
    
    for (k=0; k< 6; k++) {
      strcpy_P(buffer, (char*)pgm_read_word(&(string_table[k])));
      Serial.println(buffer);
      delay(250);
    }
    */
    
    unsigned int pixelNumber = 0;
    Pixel pixel;
    //uint16_t pixelSet = pixels_from_red;
    pixel = readPixelPackage(pixelNumber, pixels);
    
    // set up pixels
    for (unsigned int index = 0; index < NUM_PIXELS; index++) {
      pixelPointers[index].fromPixelIndex = rand() % numPixelColors;
      pixelPointers[index].toPixelIndex = rand() % numPixelColors;
      
      Pixel p;
      p = readPixelPackage(pixelPointers[index].toPixelIndex, pixels);
      pixelPointers[index].timeout = p.timeout;
    }
    
    printPixel(pixel);
    lastTime = millis();
    
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}


void loop() {
  lastTime = currentTime;
  currentTime = millis();
  
  // go through each pixel
  // push towards the timeout with the tween algorithm
  // if the timeout is expired, reset the pixel
  for (unsigned int index = 0; index < NUM_PIXELS; index++) {
    if (pixelPointers[index].timeout <= 0) {
      Pixel p;
      pixelPointers[index].fromPixelIndex = pixelPointers[index].toPixelIndex;
      pixelPointers[index].toPixelIndex = rand() % numPixelColors;
      p = readPixelPackage(pixelPointers[index].toPixelIndex, pixels);
      pixelPointers[index].timeout = p.timeout;
    }
    
    // subtract the time difference
    pixelPointers[index].timeout = pixelPointers[index].timeout - (currentTime - lastTime);
    
    // tween between pixels
    Pixel startPixel = readPixelPackage(pixelPointers[index].fromPixelIndex, pixels);
    Pixel endPixel = readPixelPackage(pixelPointers[index].toPixelIndex, pixels);
    
    hsv currentPixelColor = tweenPixel(startPixel, endPixel, pixelPointers[index].timeout);
    rgb currentPixelColorRGB = hsv2rgb(currentPixelColor);
    
    strip.setPixelColor(index, strip.Color(currentPixelColorRGB.r, currentPixelColorRGB.g, currentPixelColorRGB.b));
    //strip.setPixelColor(index, strip.Color(rand()%255, rand()%255, rand()%255));
  }
  strip.show();
}


hsv tweenPixel(Pixel startPixel, Pixel endPixel, unsigned int timeout) {
  hsv currentColor;
  
  float percentDone = 1-(100.0*timeout/(endPixel.timeout*8));
  
  currentColor.h = ((endPixel.fromColor.h - startPixel.fromColor.h)*percentDone + startPixel.fromColor.h)/100;
  currentColor.s = ((endPixel.fromColor.s - startPixel.fromColor.s)*percentDone + startPixel.fromColor.s)/100;
  currentColor.v = ((endPixel.fromColor.v - startPixel.fromColor.v)*percentDone + startPixel.fromColor.v)/100;
  
  return currentColor;
}

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

Pixel readPixelPackage(unsigned int pixelNumber, const long unsigned int* memoryAddress) {
  int format = BIN;
  /*
  Pixel pixel;
  
  unsigned int memoryOffset = pixelNumber*64;
  int carat = 0;
  
  int increment = 8;
  
  
  // 11111111100000000110000001000000 100000000100000000010111011100
  
  Serial.print(0xFF806040, format);
  Serial.print(" ");
  Serial.print(0x201005DC, format);
  Serial.println();
  /* */
  
  unsigned long package1 = pgm_read_dword_near(memoryAddress+(pixelNumber*2));
  unsigned long package2 = pgm_read_dword_near(memoryAddress+(pixelNumber*2)+1);
  
  Serial.print(package1, format);
  Serial.print(" ");
  Serial.println(package2, format);
  /* */
  Pixel pixel = unpackPixel(package1, package2);
  
  return pixel;
}


Pixel unpackPixel(unsigned long package1, unsigned long package2) {
  Pixel pixel;
  
  pixel.timeout = package2 & 0xFFFF;
  pixel.toColor.v = (package2 >> 16) & 0xFF;
  pixel.toColor.s = (package2 >> 24) & 0xFF;
  
  pixel.toColor.h = package1 & 0xFF;
  pixel.fromColor.v = (package1 >> 8) & 0xFF;
  pixel.fromColor.s = (package1 >> 16) & 0xFF;
  pixel.fromColor.h = (package1 >> 24) & 0xFF;

  
  return pixel;
}
/* */


/*
void printPixel(Pixel pixel) {
  int format = DEC;
  
  Serial.println("Pixel:");
  Serial.println("============");
  Serial.print("from h: ");
  Serial.println(pixel.fromColor.h, format);
  Serial.print("from s: ");
  Serial.println(pixel.fromColor.h, format);
  Serial.print("from v: ");
  Serial.println(pixel.fromColor.v, format);
  
  Serial.print("to h: ");
  Serial.println(pixel.toColor.h, format);
  Serial.print("to s: ");
  Serial.println(pixel.toColor.h, format);
  Serial.print("to v: ");
  Serial.println(pixel.toColor.v, format);
  
  Serial.print("timeout: ");
  Serial.println(pixel.timeout, format);
}
/* */
