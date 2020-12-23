#define SECONDS 1000
#define MINUTES = 60

#include "lightSensor.h"
#include "lights.h"
//#include "pir.h"


static uint8_t LIGHT_TIMEOUT_S = 1; // 2-5 minutes
static uint8_t LIGHT_SENSOR_TIMEOUT_S = 50; // 5

static float GAMMA_MIN = 128;  // 96-128?
static float GAMMA_MAX = 255; // 255

static float AMBIENT_BRIGHTNESS_MIN = 100;  
static float AMBIENT_BRIGHTNESS_MAX = 300; 



int blinkPin = 0;



int getGammaFromAmbientLight(float ambientLightSensorValue) {
  float ambientLightRange = AMBIENT_BRIGHTNESS_MAX - AMBIENT_BRIGHTNESS_MIN;
  float gammaRange = GAMMA_MAX - GAMMA_MIN;

  float slope = gammaRange / ambientLightRange;
  float offset = -AMBIENT_BRIGHTNESS_MIN * slope + GAMMA_MIN;

  float gamma = slope * ambientLightSensorValue +  offset;
  gamma = min(gamma, GAMMA_MAX);
  gamma = max(gamma, GAMMA_MIN);

/*
  Serial.print("ambientLightSensorValue: ");
  Serial.print(ambientLightSensorValue);
  Serial.print("\t");
  Serial.print("gamma: ");
  Serial.println(gamma);
  */

  return (int)gamma;
}


void setup()
{
  pinMode(blinkPin, OUTPUT);
  setupLightSensor();
}


unsigned long lightTimeout_ts = 0;
unsigned long lightSensorTimout_ts = 0;
void loop()
{
  
//  loopPir();
  
  loopLightSensor();
  //int lightValue =  map(ambientLightSensorValue, 0, 1023, 0, 255);


  unsigned long currentTime_ts = millis();
  if (currentTime_ts > lightSensorTimout_ts) {
    lightSensorTimout_ts = currentTime_ts + (LIGHT_SENSOR_TIMEOUT_S * SECONDS);
    loopLightSensor();
    lightBrightnessMax = getGammaFromAmbientLight(ambientLightSensorValue);
  }

  
  int pirState = digitalRead(1);
  //digitalWrite(blinkPin, pirState);


  if (pirState == HIGH) {
      turnLightsOn();
      //Serial.println("Motion Sensed");
      lightTimeout_ts = millis() + (LIGHT_TIMEOUT_S * SECONDS);
  } else {
      turnLightsOff();
  }

  
  loopLights();

  
//  analogWrite(blinkPin, lightValue);
}