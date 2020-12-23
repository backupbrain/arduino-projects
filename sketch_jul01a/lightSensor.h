static uint8_t lightSensorPin = A1;


static float SENSOR_RANGE = 1024; // 3.3v
static float LUX_LOG_RANGE = 5.0; // 3.3v = 10^5 lux

//float ambientLight_lux = 0.0;
int ambientLightSensorValue = 0;

/**
 Luminocity Chart
 https://learn.adafruit.com/adafruit-ga1a12s202-log-scale-analog-light-sensor/use-it
 
     0.002  lux Moonless clear night sky
     0.2    lux Design minimum for emergency lighting (AS2293).
     0.27   1 lux  Full moon on a clear night
     3.4    lux Dark limit of civil twilight under a clear sky
    50      lux  Family living room
    80      lux  Hallway/toilet
   100      lux Very dark overcast day
   300 - 500 lux Sunrise or sunset on a clear day. Well-lit office area.
 1,000      lux Overcast day; typical TV studio lighting
10,000 - 25,000 lux Full daylight (not direct sun)
32,000 -130,000 lux  Direct sunlight
 */

 
/*
float getLuxFromSensorValue(int sensorValue) {
  float logLux = sensorValue * LUX_LOG_RANGE / SENSOR_RANGE;
  return pow(10, logLux);
}
*/

void setupLightSensor() {
  //analogReference(EXTERNAL); // if we are using another analog reference
  pinMode(lightSensorPin, INPUT);
  digitalWrite(lightSensorPin, LOW);
}


void loopLightSensor() {  
  ambientLightSensorValue = analogRead(lightSensorPin);   

  /*
  ambientLight_lux = getLuxFromSensorValue(sensorValue);
  
  Serial.print("Raw = ");
  Serial.print(sensorValue);
  Serial.print(" - Lux = ");
  Serial.println(ambientLight_lux); 
  delay(100);
  */
  
}



