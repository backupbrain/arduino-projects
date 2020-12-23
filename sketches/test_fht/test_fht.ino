    
#include "FunctionGenerator.h"

#define LOG_OUT 1 // use the log output function
#define OCTAVE 1 // use the fft_mag_octave output function
#define FHT_N 256 // set to 256 point fht 
#include <FHT.h>

const unsigned int SAMPLE_RATE = 44100;
const unsigned int NUM_FREQUENCIES = 4;

const unsigned int NUM_SAMPLES = 256;

//int fht_input[NUM_SAMPLES];

int output = 0;

FunctionGenerator functionGenerator(NUM_FREQUENCIES);

void setup() {
  Serial.begin(115200);
  functionGenerator.addFunction(256, .25, 0);
  functionGenerator.addFunction(64, 10, 0);
  functionGenerator.addFunction(96, 2, 0);
  functionGenerator.addFunction(128, 0.5, 0);
}

void loop() {
  
  for (int sample=0; sample<NUM_SAMPLES; sample++) {
    fht_input[sample] = functionGenerator.getSnapshot()-functionGenerator.BASELINE;
  }
  
 // process data
  fht_window(); // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  //fht_mag_octave(); // take the output of the fht, save into fht_oct_out[]
  // this will result in log(N) bins
  // apparently the input is takes from fft_input?
  
  // alternatevly, can use fht_mag_lin, which returns fht_lin_out[] linearly
  
  fht_mag_log(); // take the output of the fht
  //fft_mag_octave();
  
   for (byte i = 0 ; i < NUM_SAMPLES/2 ; i++) {
      Serial.print(fht_log_out[i]);
      //Serial.print(fft_oct_out[i]);
      Serial.print(" ");
   }
   Serial.println();
  /*
  uint8_t arr[] = fht_lin_out;
  for (int frequencyIndex = 0; frequencyIndex < NUM_SAMPLES/2; frequencyIndex++) {
    Serial.print(arr[frequencyIndex]);
     Serial.print(" "); 
  }
  Serial.println();
  /* */
  /*
  binNum = findMax(fht_oct_out, FHT_N/2);
  freq = binNum * (fs / FHT_N);
  
  return freq;
  */
  
  Serial.write(255);
  Serial.write(fht_log_out, NUM_SAMPLES/2);
  
}


