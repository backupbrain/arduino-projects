//importing library for lights and pixels
#include <Adafruit_NeoPixel.h>
#include "gamma_correction.h"

//variable declarations

unsigned int daddy = 13;
unsigned long mommy = 50;
const unsigned int max_brightness = 255;
const unsigned int min_brightness = 0;
const unsigned int pin = 5;

// const means i promise 41 won't change 
// inside_pixels is the number of pixels available potentially
// but only if you want

const unsigned int inside_pixels = 41;
unsigned int interior_ring[inside_pixels];
const unsigned int first_pixel = 0;

// the first pixel is called zero
// this is the reference, "pointer" to the first pixel
// we could say unsigned int pixel = 1; for the second pixel
// but we don't need to do this

unsigned int current_pixel = first_pixel;
unsigned int tail = 4;

// magic shit i stole and brought down here bc variables above

Adafruit_NeoPixel strip = Adafruit_NeoPixel(inside_pixels, pin, NEO_GRB + NEO_KHZ800);

// DECLARATIONS AND VARIABLES ABOVE ALL THIS
// VOID SETUP SHIT BELOW ALL THIS

void setup() {
  // put your setup code here, to run once:
  // the below is just telling pin 13 daddy to be the output
  pinMode(daddy, OUTPUT);
 
  // setting my baud rate to 9600 so that computer knows how quick
  // to receive or send data
  Serial.begin(9600);

  //tell the arduino to setup the pixel strip/ boot it up

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
}


// THIS IS THE LOOP AREA BELOW ALL OF THIS
// THIS IS THE STATE OF ALL THE LIGHTS AT ANY GIVEN RUN OF THE
// CHIP'S LOOP - HENCE LOOP


void loop() {

  // once the pixel number reaches the max pixels available
  // which is 41, then we reset back to the first pixel
 
  if (current_pixel > inside_pixels) {
    current_pixel = 0;
  }
  
  // reset colors
  for (int p=0; p<inside_pixels;p++) {
    interior_ring[p] = 0;
  }
  /* */
  // okay, within color the specific pixel we're talking to
  // has a value of 255 for the brightness

  interior_ring[current_pixel] = max_brightness;
  int tail_edge = current_pixel - tail;
  
  int num_pixels_left = tail;
  bool tail_edge_wraps = (tail_edge < 0);
  if (tail_edge_wraps) {
    int num_tail_pixels_left = 0 - tail_edge;
    while (num_tail_pixels_left > 0) {
      interior_ring[inside_pixels - num_tail_pixels_left] = (max_brightness - min_brightness) * (tail - num_pixels_left) / tail;
      num_pixels_left--;
      num_tail_pixels_left--;
    }
  }
  while (num_pixels_left > 0) {
    interior_ring[current_pixel - num_pixels_left] = (max_brightness - min_brightness) * (tail - num_pixels_left) / tail;
    num_pixels_left--;
  }
 


  // this helps us cycle the pixel we're talking to
  current_pixel = current_pixel + 1;

  

  //not really analog, this outputs the power of the light in pin 13
  //to a specific variable which is whatever king is

  analogWrite(daddy, max_brightness);  
  
  //smooth that shit out a little with a slight delay, not so fast!
  
  delay(mommy);

  // we have another loop within this loop
  // p is the new way of saying pixel in this loop
  // because "pixel" is already taken as a name
  // we have this here just for printing purposes
  // for user interface purposes

  for (int p=first_pixel; p<inside_pixels; p++) {
    Serial.print (interior_ring[p]);
    Serial.print (" ");
  }
  Serial.println("");


  // yet another loop because you need new loops for different things
  // the loop above was writing shit in a monitor
  // this shit here writes shit to the actual strip with colors
 
  for (int p=first_pixel; p<inside_pixels; p++) {
    strip.setPixelColor(p,
      pgm_read_byte(&gamma8[interior_ring[p]]),
      pgm_read_byte(&gamma8[interior_ring[p]]),
      pgm_read_byte(&gamma8[interior_ring[p]]));
    // strip.setPixelColor(p, strip.Color(interior_ring[p], interior_ring[p], interior_ring[p]));
  }
  strip.show();
  /* */


  

}
