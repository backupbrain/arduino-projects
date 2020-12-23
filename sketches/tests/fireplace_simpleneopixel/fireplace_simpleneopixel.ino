
#include <avr/pgmspace.h>
#include "progmem_test.h"
#include "SimpleNeopixel.h"
#include <MemoryFree.h>

/*
 This is an example of how simple driving a Neopixel can be
 This code is optimized for understandability and changability rather than raw speed
 More info at http://wp.josh.com/2014/05/11/ws2812-neopixels-made-easy/
*/

// Change this to be at least as long as your pixel string (too long will work fine, just be a little slower)


#define NUM_PIXELS 12
#define NEOPIXEL_PIN 9
#define DIVIDER 2
const byte numWorkingPixels = NUM_PIXELS/DIVIDER;

#define numPixelColors 16
const unsigned long pixels[] PROGMEM = {
  0x7C70634,
  0xE132A0,
  0x18FB471D,
  0x17F70834,
  0x14CF361D,
  0x1CE6341D,
  0x8DE3F34,
  0xAD31C34,
  0x1BF23A1D,
  0x1DD028A0,
  0xFDD3F34,
  0x13CE0B34,
  0x21E839A0,
  0x7E20134,
  0x11DE321D,
  0x1F91334,
  /*
  0x15C21534,
  0x16E52E34,
  0x1AF22234,
  0x9CD2A34,
  /* */
};



PixelPointer pixelPointers[NUM_PIXELS];


SimpleNeopixel strip = SimpleNeopixel(PIXEL_BIT, PIXELS);



unsigned long lastTime = 0;
unsigned long currentTime = 0;

void setup() {
  
    //Serial.begin(9600);
    unsigned int pixelNumber = 0;
    Pixel pixel;
    pixel = readPixelPackage(pixelNumber, pixels);
    
    // set up pixels
    for (unsigned int index = 0; index < numWorkingPixels; index++) {
      pixelPointers[index].fromPixelIndex = rand() % numPixelColors;
      pixelPointers[index].toPixelIndex = rand() % numPixelColors;
      
      Pixel p;
      p = readPixelPackage(pixelPointers[index].toPixelIndex, pixels);
      pixelPointers[index].timeout = p.timeout;
      
      
      
    }
    
    
    strip.begin();
    
    lastTime = millis();
  
}


void loop() {
  
  //Serial.println(2560-freeMemory());
  
  
  
  lastTime = currentTime;
  currentTime = millis();
  
  
  updateFireplacePixels();
  displayPixels();
  /*
  
  // Some example procedures showing how to display to the pixels:
  colorWipe(255, 0, 0, 0); // Red
  colorWipe(0, 255, 0, 0); // Green
  colorWipe(0, 0, 255, 0); // Blue
  
  // Send a theater pixel chase in...
  theaterChase(127, 127, 127, 0); // White
  theaterChase(127,   0,   0, 0); // Red
  theaterChase(  0,   0, 127, 0); // Blue
  
  rainbowCycle(1000 , 20 , 5 );
  detonate( 255 , 255 , 255 , 1000);
  /* */
  
  
  return;
  
}


void updateFireplacePixels() {
  
  int timeDifference = currentTime - lastTime;
  // go through each pixel
  // push towards the timeout with the tween algorithm
  // if the timeout is expired, reset the pixel
  for (unsigned int index = 0; index < numWorkingPixels; index++) {
    if (pixelPointers[index].timeout <= 0) {
      pixelPointers[index].fromPixelIndex = pixelPointers[index].toPixelIndex;
      
      pixelPointers[index].toPixelIndex = rand() % numPixelColors;
      Pixel p;
      p = readPixelPackage(pixelPointers[index].toPixelIndex, pixels);
      pixelPointers[index].timeout = p.timeout;
      
      
    }
    
    // subtract the time difference
    if (timeDifference > pixelPointers[index].timeout) {
      pixelPointers[index].timeout = 0;
    } else {
      pixelPointers[index].timeout = pixelPointers[index].timeout - timeDifference;
    }
    
    
    // tween between pixels
    Pixel startPixel = readPixelPackage(pixelPointers[index].fromPixelIndex, pixels);
    Pixel endPixel = readPixelPackage(pixelPointers[index].toPixelIndex, pixels);
    
    hsv currentPixelColor = tweenPixel(startPixel, endPixel, pixelPointers[index].timeout);
    rgb currentPixelColorRGB = hsv2rgb(currentPixelColor);
    
    
  }
  strip.show();
}

void displayPixels() {
  cli();
  for (unsigned int i=0; i < DIVIDER; i++) {
    //strip.setPixelColor(index+(numWorkingPixels*i), currentPixelColorRGB.r, currentPixelColorRGB.g, currentPixelColorRGB.b);
    strip.sendPixel(currentPixelColorRGB.r, currentPixelColorRGB.g, currentPixelColorRGB.b);
  }
  sei();
}
/* */



/*

  That is the whole API. What follows are some demo functions rewriten from the AdaFruit strandtest code...
  
  https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/strandtest/strandtest.ino
  
  Note that we always turn off interrupts while we are sending pixels becuase an interupt
  could happen just when we were in the middle of somehting time sensitive.
  
  If we wanted to minimize the time interrupts were off, we could instead 
  could get away with only turning off interrupts just for the very brief moment 
  when we are actually sending a 0 bit (~1us), as long as we were sure that the total time 
  taken by any interrupts + the time in our pixel generation code never exceeded the reset time (5us).
  
*/


// Display a single color on the whole string

void showColor( unsigned char r , unsigned char g , unsigned char b ) {
  
  cli();  
  for( int p=0; p<PIXELS; p++ ) {
    strip.sendPixel( r , g , b );
  }
  sei();
  strip.show();
  
}

// Fill the dots one after the other with a color
// rewrite to lift the compare out of the loop
void colorWipe(unsigned char r , unsigned char g, unsigned char b, unsigned  char wait ) {
  for(unsigned int i=0; i<PIXELS; i+= (PIXELS/60) ) {
    
    cli();
    unsigned int p=0;
    
    while (p++<=i) {
        strip.sendPixel(r,g,b);
    } 
     
    while (p++<=PIXELS) {
        strip.sendPixel(0,0,0);  
      
    }
    
    sei();
    strip.show();
    delay(wait);
  }
}

// Theatre-style crawling lights.
// Changes spacing to be dynmaic based on string size

#define THEATER_SPACING (PIXELS/20)

void theaterChase( unsigned char r , unsigned char g, unsigned char b, unsigned char wait ) {
  
  for (int j=0; j< 3 ; j++) {  
  
    for (int q=0; q < THEATER_SPACING ; q++) {
      
      unsigned int step=0;
      
      cli();
      
      for (int i=0; i < PIXELS ; i++) {
        
        if (step==q) {
          
          strip.sendPixel( r , g , b );
          
        } else {
          
          strip.sendPixel( 0 , 0 , 0 );
          
        }
        
        step++;
        
        if (step==THEATER_SPACING) step =0;
        
      }
      
      sei();
      
      strip.show();
      delay(wait);
      
    }
    
  }
  
}
        


// I rewrite this one from scrtach to use high resolution for the color wheel to look nicer on a *much* bigger string
                                                                            
void rainbowCycle(unsigned char frames , unsigned int frameAdvance, unsigned int pixelAdvance ) {
  
  // Hue is a number between 0 and 3*256 than defines a mix of r.g.b where
  // hue of 0 = Full red
  // hue of 128 = 1/2 red and 1/2 green
  // hue of 256 = Full Green
  // hue of 384 = 1/2 green and 1/2 blue
  // ...
  
  unsigned int firstPixelHue = 0;     // Color for the first pixel in the string
  
  for(unsigned int j=0; j<frames; j++) {                                  
    
    unsigned int currentPixelHue = firstPixelHue;
       
    cli();    
        
    for(unsigned int i=0; i< PIXELS; i++) {
      
      if (currentPixelHue>=(3*256)) {                  // Normalize back down incase we incremented and overflowed
        currentPixelHue -= (3*256);
      }
            
      unsigned char phase = currentPixelHue >> 8;
      unsigned char step = currentPixelHue & 0xff;
                 
      switch (phase) {
        
        case 0: 
          strip.sendPixel( ~step , step ,  0 );
          break;
          
        case 1: 
          strip.sendPixel( 0 , ~step , step );
          break;

        case 2: 
          strip.sendPixel(  step ,0 , ~step );
          break;
          
      }
      
      currentPixelHue+=pixelAdvance;                                      
      
                          
    } 
    
    sei();
    
    strip.show();
    
    firstPixelHue += frameAdvance;
           
  }
}

  
// I added this one just to demonstrate how quickly you can flash the string.
// Flashes get faster and faster until *boom* and fade to black.

void detonate( unsigned char r , unsigned char g , unsigned char b , unsigned int startdelayms) {
  while (startdelayms) {
    
    showColor( r , g , b );      // Flash the color 
    showColor( 0 , 0 , 0 );
    
    delay( startdelayms );      
    
    startdelayms =  ( startdelayms * 4 ) / 5 ;           // delay between flashes is halved each time until zero
    
  }
  
  // Then we fade to black....
  
  for( int fade=256; fade>0; fade-- ) {
    
    showColor( (r * fade) / 256 ,(g*fade) /256 , (b*fade)/256 );
        
  }
  
  showColor( 0 , 0 , 0 );
  
    
}



hsv tweenPixel(Pixel startPixel, Pixel endPixel, unsigned int timeout) {
  hsv currentColor;
  
  float percentDone = 1-((float)timeout/(endPixel.timeout));
  
  float h = ((endPixel.color.h - startPixel.color.h)*percentDone + startPixel.color.h);
  float s = ((endPixel.color.s - startPixel.color.s)*percentDone + startPixel.color.s);
  float v = ((endPixel.color.v - startPixel.color.v)*percentDone + startPixel.color.v);
  
  currentColor.h = h;
  currentColor.s = s;
  currentColor.v = v;
  
  return currentColor;
}

Pixel readPixelPackage(unsigned int pixelNumber, const long unsigned int* memoryAddress) {
  unsigned long package = pgm_read_dword_near(memoryAddress+pixelNumber);
  Pixel pixel = unpackPixel(package);
  
  return pixel;
}


Pixel unpackPixel(unsigned long package) {
  Pixel pixel;
  
  pixel.timeout = (package & 0xFF) * 8;
  pixel.color.v = (package >> 8) & 0xFF;
  pixel.color.s = (package >> 16) & 0xFF;
  pixel.color.h = (package >> 24) & 0xFF;
  
  return pixel;
}



