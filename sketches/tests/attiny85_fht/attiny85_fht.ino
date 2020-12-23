/*
fixFFT http://forum.arduino.cc/index.php/topic,38153.0.html
 this should give an fft with 
 sampling rate:           1ms
 frequency resolution:  500Hz
 lowest frequency:       7.8Hz
 */
 
#include "Simple_Neopixel.h"
#include "AudioSampler.h"
#include "Fixed_FFT.h"
#include "FHTHistory.h"
#include "WaterfallPlotter.h"


////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE CONSTANTS
// Change these to change how the program records and processes data
////////////////////////////////////////////////////////////////////////////////

/** 
 * System Configuration
 */
const byte POWER_LED_PIN = 13;

/**
 * Audio Sampling Configuration
 */
const unsigned int AUDIO_SAMPLE_RATE_HZ = 44100;             // Sample rate of the audio in hertz.
const unsigned int AUDIO_SAMPLE_LENGTH = 128; // was 1024
const byte AUDIO_INPUT_PIN = A0;        // Input ADC pin for audio data.

/**
 * FHTHistory Configuration
 */
// 128 of 44100 samples per second = 2.9 milliseconds.  
// roughly 340 samples per second.  
// so if we want 1 second of history, then we want 340 samples
// HISTORY_BUFFER_LENGTH = sample_length_seconds * 340
// WARNING: an arduino may only have 1 or 2 kb or so of RAM to work with
float historyLength_seconds = 0.25;
const unsigned int HISTORY_BUFFER_LENGTH = 340; //(int)floor(historyLength_seconds*(AUDIO_SAMPLE_RATE_HZ/AUDIO_SAMPLE_LENGTH));

/**
 * Audio Processing Configuration
 */
const unsigned int SPECTRUM_MIN_DB = 30;
const unsigned int SPECTRUM_MAX_DB = 90;
const unsigned int MIN_SOUND_VOLUME = 0;
const unsigned int FFT_RESOLUTION = FHT_N/2;              // Size of the FFT.  Realistically can only be at most 256 

/**
 * Waterfall Plotter Configuration
 */
const unsigned int NEOPIXEL_WIDTH = 8; //30; // 8
const unsigned int NEOPIXEL_HEIGHT = 8; //10; // 8
const float MAX_BRIGHTNESS = 1.0;


int ledG = 1;//pwm
int ledR = 0;//pwm
int ledB = 2;//pwm
int mic = A1; //electret
 
char im[128];
char data[128]; 
char data_avgs[128];
 
//mix max val to map fft
int valMin = 0;
int valMax = 30;  
//bias to reduce on low and increase on high  
int bias = 0;//+- bias to output
 
//3 chn selected deliberately out of 64 bins by fft
int chnLow = 0; //blue
int chnMid = 6; //green
int chnHigh = 11; //red
 
//temp var for bands
int tempLow = 0;
int tempMid = 0;
int tempHigh = 0;
 
void setup(){
  pinMode(NEOPIXEL_PIN, OUTPUT);    
  pinMode(ledR, OUTPUT);    
  pinMode(ledB, OUTPUT);
  pinMode(MICROPHONE_PIN, INPUT);
}
 
void loop(){
  int static i = 0;
  static long tt;
  int val;
  randomSeed(analogRead(mic));
  bias = random(70,150);
 
  for (i=0; i < 128; i++){  
    val = analogRead(mic);
    //data[i] = val / 4 - 128;
	data[i] = val;//quick and dirty data.
    im[i] = 0;
    i++;   
  }//end for
 
  //this could be done with the fix_fftr function without the im array.
  Fixed_FFT::fft(data,im,7,0);
  /*
   Performs foward or inverse Fourier transform.
   //fix_fft (char fr[], char fi[], int m, int inverse)
   //fr is a real data set,
   //fi is the imaginary data set,
   // m is log2(n) where n is number of data points (log2 (128) = 7)
   //0 is set for forward transform. 1 would be inverse transform. Apparently inverse does not work,
   */
  // I am only interessted in the absolute value of the transformation
  for (i=0; i< 64;i++){//real val is for the amplitude 
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); 
  }//end for
 
  //do something with the data values 1..64 and ignore im
  //data avg moved to individual func
 
  //shin: styles for colour mapping to frequency
  //triChn(bias);
  //triChn(0);//no bias
  //style2
  //grpChnLowPass();
  //style3
  treshChn();
 
}//end loop
 
 
void triChn(int bias){//read low, mid, high chn
  for (int i=0; i<14; i++) {//
    data_avgs[i] = data[i*4] + data[i*4 + 1] + data[i*4 + 2] + data[i*4 + 3];   // average together 
    //data_avgs[i] = map(data_avgs[i], 0, 30, 0, 9);// remap values for LOL(9rowx14col led)
    data_avgs[i] = map(data_avgs[i], valMin, valMax, 0, 255);// remap values for RGB LED
  }
  //bias to give more blue to emphasise low freq
  analogWrite(ledB, data_avgs[chnLow]);
  analogWrite(ledG, data_avgs[chnMid]-(bias/2));
  analogWrite(ledR, data_avgs[chnHigh]-bias);
}
 
void grpChnLowPass(){//grp low pas 32 out of 64 bins into grp of low mid high, avg out in grp
  for (int i=0; i<14; i++) {//
    data_avgs[i] = data[i*4] + data[i*4 + 1] + data[i*4 + 2] + data[i*4 + 3];   // average together 
    //data_avgs[i] = map(data_avgs[i], 0, 30, 0, 9);// remap values for LOL(9rowx14col led)
    data_avgs[i] = map(data_avgs[i], valMin, valMax, 0, 255);// remap values for RGB LED
  }
 
  int numChn = 4;
  for (int i=0; i<numChn; i++) { //14 bins grp into3 bands
    //low chn 
    tempLow=data_avgs[i]+tempLow;
    //mid chn
    tempMid=data_avgs[(i+4)]+tempMid;
    //high chn
    tempHigh=data_avgs[i+8]+tempHigh;
  }//end for
  //avg across the grp  
  tempLow = tempLow / numChn;  
  tempMid = tempMid / numChn;
  tempHigh = tempHigh / numChn;
  //map freq to rgb on  pwm pin
 
    tempLow = map(tempLow, valMin, valMax, 0, 255);
  tempMid = map(tempMid, valMin, valMax, 0, 255);
  tempHigh = map(tempHigh, valMin, valMax, 0, 255);
 
  //output to rgb
  analogWrite(ledB, tempLow);
  analogWrite(ledG, tempMid);
  analogWrite(ledR, tempHigh);
 
}//grpChnLowPass
 
void treshChn(){//on if over threshold low, mid, high
  for (int i=0; i<14; i++) {//only take lower half of real freq band eg 32 out of 64
    data_avgs[i] = data[i*4] + data[i*4 + 1] + data[i*4 + 2] + data[i*4 + 3];   // average together 
    //data_avgs[i] = map(data_avgs[i], 0, 30, 0, 9);// remap values for LOL(9rowx14col led)
    data_avgs[i] = map(data_avgs[i], valMin, valMax, 0, 255);// remap values for RGB LED
  }
 
  tempLow = data_avgs[chnLow];
  tempMid = data_avgs[chnMid];
  tempHigh =data_avgs[chnHigh];
 
  if(tempLow>150){
    //more low gives blue
    analogWrite(ledB, tempLow);
    analogWrite(ledG, 255);
    analogWrite(ledR, 255);
    //delay(100);
  }
 
  else if (tempMid>150){
    //more mid gives green
    analogWrite(ledB, 255);
    analogWrite(ledG, tempMid);
    analogWrite(ledR, 255);
    //delay(100);
  }
  else if (tempHigh>150){
    //more high gives red
    analogWrite(ledB, 255);
    analogWrite(ledG, 255);
    analogWrite(ledR, tempHigh);
    //delay(100);
  }
  else{
    /*
    //random colour not visual appealing to music
     tempLow=random(0,255);
     tempMid=random(0,255);
     tempHigh=random(0,255);
     */
    analogWrite(ledB, tempLow-70);
    analogWrite(ledG, tempMid-100);
    analogWrite(ledR, tempHigh-150);
    //delay(100);
  }  
 
}//end treshChn

