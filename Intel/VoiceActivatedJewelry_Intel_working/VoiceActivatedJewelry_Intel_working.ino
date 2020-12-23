/**
 * This sketch should produce only red pixels, 
 * but for some reason the first pixel is bright green
 * 
 */
#define N_LEDS 16
 
/**
 * Heartbeat
 */
const unsigned long kHeartBeatPeriod = 800;
bool heartbeat_active = false;
bool heartbeatBoot = false;
unsigned long heartbeatStartTime_ms = 0;

#include "Adafruit_NeoPixel.h"
#include "gammaCorrection.h"
#include "visualizations.h"
#include "PulseTimer.h"
#include "pushbutton.h"
#include "bluetooth.h"
#include "Heartbeat.h"
#include "bootsequence.h"



/**
 * Pushbutton
 */
static const uint8_t pushButtonPin = A1;
static const unsigned long buttonDebounceTimeout_ms = 50;
PulseTimer buttonDebounceTimer = PulseTimer();
bool lastButtonState = false;

/**
 * Slide switch
 */
static const uint8_t userSwitchPin = 4;


/**
 * Power Pins
 */
static const uint8_t sensorsPowerPin = 8;
static const uint8_t ledRingPowerPin = 7;

/**
 * User button
 */
const uint8_t USER_BUTTON_PIN = 2;
boolean lastUserButtonValue;

/** 
 *  Audio setup
 */
const byte micPin = A0;
const byte numMicSamples = 25; // Sample window width in mS (50 mS = 20Hz)
const byte numVolumeSamples = 10;

/** 
 *  LED constants
 */
const unsigned int numPixels = N_LEDS;
const byte neoPixelPin = 6;


/**
 * Internal state - do not change anything below this line
 */
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);
#define arraySize(array) ((sizeof(array)/sizeof(0[array])) / ((size_t)(!(sizeof(array) % sizeof(0[array])))))
byte volumeHistory[numVolumeSamples];
const unsigned int baudRate = 9600;
byte lastBrightness = 0;



void setupUserButton() {
  buttonDebounceTimer.setDelay(buttonDebounceTimeout_ms);
  pinMode(USER_BUTTON_PIN, INPUT_PULLUP);
  lastUserButtonValue = HIGH;
}


/**
 * This function gets the signal amplitude of an analog microphone over sampleWindaw samples
 * 
 * @param micPin the pin to access the microphone.  Must be an ADC pin
 * @param sampleWindow the number of samples to take. More samples means longer averages but also more time consumed
 * @return the signal amplitude over the sampleWindow envelope
 */
unsigned int getMicVolume(byte micPin, unsigned int numSamples) {
   
  
   unsigned long startMillis = millis();  // Start of sample window
   unsigned int signalAmplitude = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;

   unsigned int sample;

 
   // collect data for 50 mS
   while (millis() - startMillis < numSamples) {
      sample = analogRead(micPin);
      if (sample < 1024) {
         if (sample > signalMax) {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin) {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   signalAmplitude = signalMax - signalMin;  // max - min = peak-peak amplitude

   return signalAmplitude;
   
}


/**
 * Programatically adjust the gain by calculating the average 
 * mic volume over a period of samples
 * 
 * @param volumeHistory the sample volume window
 * @param newVolume the newest volume to put in 
 * @return the average volume over the envelope
 */
byte getAverageAmplitude(byte newVolume, byte volumeHistory[]) {
  unsigned int arrayLength = arraySize(volumeHistory);
  for (int i=arrayLength-1; i>0; i--) {
    volumeHistory[i] = volumeHistory[i-1];
  }
  volumeHistory[0] = newVolume;

  float averageVolume = 0.0;
  for (int i = 0; i < arrayLength; i++) {
    averageVolume += volumeHistory[i];
  }
  averageVolume /= arrayLength;
  
  return averageVolume;
}


void renderPixels(Adafruit_NeoPixel& strip, byte averageAmplitude) {
  byte r, g, b;
  float floatValueDisruptsStuff;
  
  // color all pixels black
  /*
  for (int i=0; i<strip.numPixels(); i++) {
    floatValueDisruptsStuff =  16 * (sin(millis()/1000.0f)*0.5f + 0.5f);
    strip.setPixelColor(i, strip.Color((int)floatValueDisruptsStuff, 0, 0));
  }
  /* */
  
 for (int i = 0; i < strip.numPixels(); ++i) {
     unsigned long time_now = millis();
     double time_seconds = time_now / 1000.0;
     
     r = averageAmplitude * (sin(i*3 + time_seconds*6.0) * .5 + .5);
     //g = 32 * (sin(i + time_seconds*6.0) * .5 + .5);
     g = 0;
     b = averageAmplitude * (sin(i*5.0 + time_seconds*8.0) * .5 + .5);
  
     strip.setPixelColor(i, r, g, b);
  }
  
  //strip.setPixelColor(0, 0, 0, 0);
  strip.show();
}



/**
 * Listen for user input
 * on button press, change the visualization
 */
PulseTimer buttonTimer = PulseTimer();
void listenForUserButton() {
  boolean buttonPress = false;
  boolean currentUserButtonValue = digitalRead(USER_BUTTON_PIN);
    //Serial.print("Button: ");
    //Serial.println(currentUserButtonValue);
  if (currentUserButtonValue < lastUserButtonValue) {
    buttonPress = true;
  }
  lastUserButtonValue = currentUserButtonValue;  
  
  if (buttonPress) {
      visualization++; 
      if (visualization >= VISUALIZATION_DEFAULT + NUM_VISUALIZATIONS) {
        visualization = VISUALIZATION_DEFAULT;
      }
      if (visualization < VISUALIZATION_DEFAULT) {
        visualization = VISUALIZATION_DEFAULT;
      }
  }

  buttonTimer.setDelay(20);
}


void listenForLightsButton() {
  buttonDebounceTimer.setDelay(buttonDebounceTimeout_ms);
  if (buttonDebounceTimer.Update()) {
  
    bool buttonState = readPushButton(pushButtonPin);
    if (buttonState != lastButtonState) {
      lastButtonState = buttonState;
      Serial.println(buttonState);
      if (buttonState == true) {
        sendChangeLightModeCommand((const unsigned char*) changeLightModeCommand, 17);
      }
    } 
  }
}

int lastSwitchValue = 0;
void listenForSlideSwitch() {
  int value = digitalRead(userSwitchPin);
  if (value != lastSwitchValue) {
    lastSwitchValue = value;
    if (value <= 0) {
      heartbeat_active = false;
      setHeartbeatDisabled();
    } else {
      heartbeat_active = true;
      setHeartbeatEnabled();
      heartbeatBoot = true;
    }
  
  }
}


void setup() {
  Serial.begin(baudRate);
  setup_bluetooth();
  pushbutton_setup(pushButtonPin);

  // turn on sensors  
  pinMode(sensorsPowerPin, OUTPUT);
  pinMode(ledRingPowerPin, OUTPUT);
  pinMode(userSwitchPin, INPUT);
  digitalWrite(sensorsPowerPin, HIGH);
  digitalWrite(ledRingPowerPin, HIGH);
  
  strip.begin();

  // this for some reason produces a green pixel 
  strip.show();
  
  initializeHeatMap(strip);
  setupUserButton();
  
  setup_bootsequence(&strip);

}


/**
 * Local Functions
 */

void RunHeartBeat(unsigned long cycle_length_ms,
                  Adafruit_NeoPixel* strip) {
  int n = strip->numPixels();
  int time_val = millis() - heartbeatStartTime_ms;
  float heart_val = OneHeartBeat(time_val, cycle_length_ms);
    
  int red_power = int(heart_val * 255.f);

  for (int i = 0; i < n; ++i) {
    strip->setPixelColor(i, red_power, 0, 0);
  }
}



void loop() {

  listenForLightsButton();
  
  listenForUserButton();

  listenForSlideSwitch();
  
  int micSignalAmplitude = getMicVolume(micPin, numMicSamples);
  float averageAmplitude = getAverageAmplitude(micSignalAmplitude, volumeHistory);
  byte gammaCorrectedBrightness = gammaCorrect(averageAmplitude);

  bool darkForTwoFrames = false;
  if ((gammaCorrectedBrightness == 0) && (lastBrightness == 0)) {
    darkForTwoFrames = true;
  }

//gammaCorrectedBrightness = 255;

  if (heartbeat_active) {
    if (heartbeatBoot) { 
      heartbeatBoot = false;     
      heartbeatStartTime_ms = millis();
    }
    
    //tap to enable heart beat; tap again to disable
    RunHeartBeat(kHeartBeatPeriod, &strip);
    strip.show();
  } else {
  switch (visualization) {
    case VISUALIZATION_COMET_1:
      visualization_comet(strip, gammaCorrectedBrightness, 1);
      break;
    case VISUALIZATION_COMET_2:
      visualization_comet(strip, gammaCorrectedBrightness, 2);
      break;
    case VISUALIZATION_COMET_3:
      visualization_comet(strip, gammaCorrectedBrightness, 3);
      break;
    case VISUALIZATION_COMET_4:
      visualization_comet(strip, gammaCorrectedBrightness, 4);
      break;
    case VISUALIZATION_SPARKS:
      visualization_sparks(strip, gammaCorrectedBrightness);
      /*
      break;
    case VISUALIZATION_TEST:
      visualization_sparks(strip, gammaCorrectedBrightness);
      */
  }
  }

      /*
  if (darkForTwoFrames) {
    delay(30);
  } else {
    renderPixels(strip, gammaCorrectedBrightness);
    delay(5);
  }
  /* */

  
  loop_bluetooth();

}
