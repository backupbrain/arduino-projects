#include "hsvrgb.h"
#include "PulseTimer.h"


/**
 * visualization stuff
 */
uint8_t heatMap[N_LEDS];    // the "heat" map of the pixels
uint8_t maxBrightness = 128; // max brightess 0<x<255
uint8_t minBrightness = 0;  // min brightness 0<x<255
boolean decaying = false;  // refers to the state of brightening a pixel
boolean sparking = true;   // refers to the state of dimming a pixel

void initializeHeatMap(Adafruit_NeoPixel& ledRing) {
  for (int index=0; index<ledRing.numPixels(); index++) {
    heatMap[index] = minBrightness;
  }
}

//const unsigned int VISUALIZATION_TEST = 999;
const unsigned int VISUALIZATION_SPARKS = 1000;
const unsigned int VISUALIZATION_COMET_1 = 1001;
const unsigned int VISUALIZATION_COMET_2 = 1002;
const unsigned int VISUALIZATION_COMET_3 = 1003;
const unsigned int VISUALIZATION_COMET_4 = 1004;
const unsigned int VISUALIZATION_DEFAULT = VISUALIZATION_SPARKS; // points to first visualization
const unsigned int NUM_VISUALIZATIONS = 5;

unsigned int visualization = VISUALIZATION_DEFAULT;

  
/**
 * render the heat map into red colors on the ledRing
 */
float currentHue = 0.0f;
float hueAdjustSpeed = 0.5;
void renderRed(Adafruit_NeoPixel& ledRing, byte gammaCorrectedBrightness) {
  // loop through our heat map and render it to pixels as red
  //Corrected = 255 * (Image/255)^(1/2.2)
  uint8_t gammaCorrected = 0;
  currentHue += hueAdjustSpeed;
  if (currentHue > 254) {
    currentHue = 0;
  }
  HsvColor hsv;
  hsv.h = (byte) currentHue;
  hsv.s = 255;

  for (int index=0; index<ledRing.numPixels(); index++) {
    gammaCorrected = gammaCorrect(heatMap[index]);
    hsv.v = (byte) (((int)gammaCorrected * (int)gammaCorrectedBrightness)/2); 
    RgbColor rgb = HsvToRgb(hsv);
    ledRing.setPixelColor(index, ledRing.Color(rgb.r, rgb.g, rgb.b));
  }
  
  ledRing.show();
}




void visualization_test(Adafruit_NeoPixel& ledRing, byte gammaCorrectedBrightness) {  
  // iterate through each pixel
  boolean sparked = false;
  for (int index=0; index<ledRing.numPixels(); index++) {
      heatMap[index] = 255;
  }
  
  renderRed(ledRing, gammaCorrectedBrightness);
  
}


boolean heatMapSparkStates[N_LEDS]; // track which lights are coming on or fading
uint8_t sanimationSpeed_ms = 20;
PulseTimer sparktimer = PulseTimer();
void visualization_sparks(Adafruit_NeoPixel& ledRing, byte gammaCorrectedBrightness) {
  sparktimer.setDelay(sanimationSpeed_ms);
  
  if (!sparktimer.Update()) {return;}
  
  unsigned int sparkingProbability = 3;
  int decayAmount = 5;
  int sparkAmount = 20;
  
  
  // iterate through each pixel
  // each pixel has a chance of sparking (lighting up to full brigthness)
  // if that does not happen, the light decays linearly
  boolean sparked = false;
  for (int index=0; index<ledRing.numPixels(); index++) {
    sparked = (rand() % 100) < sparkingProbability;
    if (sparked) {
      heatMapSparkStates[index] = sparking;
    }
    if ((heatMapSparkStates[index] == sparking) && (heatMap[index] >= maxBrightness)) {
      heatMapSparkStates[index] = decaying;
    }
    
    if (heatMapSparkStates[index] == sparking) {
      int brightness =  heatMap[index] + sparkAmount;
      if (brightness > maxBrightness) {
        brightness = maxBrightness;
      }
      heatMap[index] = brightness;
      
    } else {
      int brightness =  heatMap[index] - decayAmount;
      if (brightness < minBrightness) {
        brightness = minBrightness;
      }
      heatMap[index] = brightness;
    }
    
  }
  /* */
  
  renderRed(ledRing, gammaCorrectedBrightness);
  
}


boolean cometHeatMap[N_LEDS];
uint8_t pos = 0;
uint8_t animationSpeed_ms = 10;
PulseTimer timer = PulseTimer();
PulseTimer movementTimer = PulseTimer();
void visualization_comet(Adafruit_NeoPixel& ledRing, byte gammaCorrectedBrightness, int numPoints = 3) {
  timer.setDelay(animationSpeed_ms);
  if (!timer.Update()) {return;}
  
  boolean sparking = true;
  boolean decaying = false;
  
  uint8_t sparkAmount = 5*numPoints;
  uint8_t decayAmount = 2*numPoints;
  
  boolean sparked = false;
  int numPixels = ledRing.numPixels();
  
  
  
  movementTimer.setDelay(150*numPoints);
  if (movementTimer.Update()) {
    pos++;
    if (pos >= numPixels) {
      pos = 0;
    }
  }
  
  // get the hotspot indeces
  uint8_t hotspots[numPoints];
  for (int hotspot=0; hotspot < numPoints; hotspot++) {
    hotspots[hotspot] = (pos+(hotspot+1)*numPixels/numPoints) % numPixels;
    //SP(hotspots[hotspot]);
    //SP(" ");
  }
  
  for (int index=0; index<numPixels; index++) {
    sparked = false;
    for (int hotspot=0; hotspot<numPoints; hotspot++) {
      if (index == hotspots[hotspot]) {
        sparked = true;
        break;
      }
    }
    
    if (sparked) {
       cometHeatMap[index] = sparking;
    }
    if ((cometHeatMap[index] == sparking) && (heatMap[index] >= maxBrightness)) {
       cometHeatMap[index] = decaying;
    }
    
    
    //SP(cometHeatMap[index]);
    //SP(" ");
    
    // color the pixels
    if (cometHeatMap[index] == sparking) {
      int brightness =  heatMap[index] + sparkAmount;
      if (brightness > maxBrightness) {
        brightness = maxBrightness;
      }
      heatMap[index] = brightness;
      
    } else {
      int brightness =  heatMap[index] - decayAmount;
      if (brightness < minBrightness) {
        brightness = minBrightness;
      }
      heatMap[index] = brightness;
    }
  } 
   
  //SPLN();
  
  
  renderRed(ledRing, gammaCorrectedBrightness);
   
}






const int STATE_ALARM_WARNING = 100;
const int STATE_ALARM_SWEEP = 103;
const unsigned long warningDuration_ms = 4000; //6000;
const int warningFrequency = 600;
const unsigned long warningPeriod_ms = 1000;
PulseTimer warningPeriodTimeout = PulseTimer();
PulseTimer warningTimeout = PulseTimer();
bool alarmState = 0;
const int sweepFrequencyStart = 400;
const int sweepFrequencyMax = 300;
const unsigned long sweepDelay_ms = 50;
const int sweepIncrementor = 25;
const int numSweepFrequencies = 3;
PulseTimer sweepTimer = PulseTimer();
int currentAlarmState = STATE_ALARM_WARNING;
int sweepFrequency;
bool warningSounding = false;


void playTone(int tone_, int speakerOut, int duration_us) {
  long elapsed_time = 0;
  if (tone_ > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration_us) {

      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(tone_ / 2);

      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(tone_ / 2);

      // Keep track of how long we pulsed
      elapsed_time += (tone_);
    } 
  }
  /*
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration_us);  
    }                                
  } 
  */                                
}

const int speakerPin = 5;
unsigned int toneDuration_us = 1000;
void soundAlarm() {
  if (currentAlarmState == STATE_ALARM_WARNING) {
    if (warningPeriodTimeout.Update()) {
      warningSounding = !warningSounding;
    } else {
      if (warningSounding) {
        playTone(warningFrequency, speakerPin, toneDuration_us);
        //buzzer.setPeriod(warningFrequency);
        //buzzer.enable();
      } else {
      }
    }
        
  
    if (warningTimeout.Update()) {
      currentAlarmState = STATE_ALARM_SWEEP; 
    }
    
  } else { // sweep mode
  
    Serial.println("alarm state\r\n");
  
    //printf("in run state\r\n");

    // switch frequency after sweep delay
    if (sweepTimer.Update()) {
      sweepFrequency -= sweepIncrementor;
      
      if (sweepFrequency < sweepFrequencyMax) {
        sweepFrequency = sweepFrequencyStart;
      }
    
    }

    playTone(sweepFrequency, speakerPin, toneDuration_us);
    //buzzer.setPeriod(sweepFrequency); 
    //buzzer.enable(); 
  }
}


const unsigned long alarmStateDelay_ms = 125;
static const int MAX_BRIGHTNESS = 127;
static const int MIN_BRIGHTNESS = 0;
PulseTimer alarmStateTimer = PulseTimer();
void visualization_alarm(Adafruit_NeoPixel& ledRing) {
  warningTimeout.setDelay(warningDuration_ms);
  warningPeriodTimeout.setDelay(warningPeriod_ms);
  sweepTimer.setDelay(sweepDelay_ms);
  alarmStateTimer.setDelay(alarmStateDelay_ms);
  
  soundAlarm();

  if (alarmStateTimer.Update()) {
    alarmState = !alarmState;
  }
  

  // half of the pixels are red, half are blue.
  
  // alternate every other series of frames
  int halfPixels = ledRing.numPixels()/2;
  int redStart = 0;
  int redEnd = halfPixels;
  int blueStart = halfPixels;
  int blueEnd = ledRing.numPixels();
  if (alarmState == 0) {
    blueStart = 0;
    blueEnd = halfPixels;
    redStart = halfPixels;
    redEnd = ledRing.numPixels();
  }
  
  // red pixels
  for (int index=redStart; index<redEnd; index++) {
      ledRing.setPixelColor(index, MAX_BRIGHTNESS, 0, 0);
  }
  for (int index=blueStart; index<blueEnd; index++) {
    ledRing.setPixelColor(index, 0, 0, MAX_BRIGHTNESS);
  }
  
  ledRing.show();
  
}

/* */

