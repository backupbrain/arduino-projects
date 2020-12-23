/**
 * This sketch takes microphone input and renders pretty colors on a neopixel strip.
 * 
 * It was designed to run on Arduino Uno, Leonardo, and Intel Curie.
 * 
 * Based on Zack Vorhies (z@zackees.com) prototype Zackees Fashion Glow Jewelry mock-up
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2016-01-26
 */


/** 
 *  Audio setup
 */
const byte micPin = A0;
const byte numMicSamples = 25; // Sample window width in mS (50 mS = 20Hz)
const byte numVolumeSamples = 10;

/** 
 *  LED constants
 */
const unsigned int numPixels = 8;
const byte neoPixelPin = 6;


/**
 * Internal state - do not change anything below this line
 */
#define arraySize(array) ((sizeof(array)/sizeof(0[array])) / ((size_t)(!(sizeof(array) % sizeof(0[array])))))
byte volumeHistory[numVolumeSamples];
const unsigned int baudRate = 9600;
unsigned long lastTime_ms = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);

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


/**
 * Renders an array of data onto pixels
 * 
 * @param strip the neopixel strip
 * @param brigtness the brightness to interpret
 * @param currentTime_ms the current time in ms
 */
void drawPixels(Adafruit_NeoPixel& strip, float intensity, unsigned long currentTime_ms) {  
  byte red, green, blue;
  for (int i = 0; i < strip.numPixels(); i++) {
    double time_seconds = currentTime_ms / 1000.0;
    // there are 2pi radians in sin, or 360 degrees
    // we want it to take 6 seconds to cycle through the brightness
    // when dealing with milliseconds, we want no increment the timeFactor every 16 milliseconds
    unsigned long timeFactor = currentTime_ms / 16;
    red = intensity * float(isin(i*3 + currentTime_ms/16) + 127)/255.0;
    green = float(isin(i + currentTime_ms/16) + 127)/255.0;
    blue = intensity * float(isin(i*5 + currentTime_ms/22) + 127)/255.0;
    strip.setPixelColor(i, red, green, blue);
  }
  strip.show();
}

/**
 * Set up the sketch
 */
void vj_setup() {
  Serial.begin(baudRate);
  pinMode(micPin, INPUT);
  strip.begin();

  // initialize pixels to black, hopefully will remove the green pixel at start of program
  for (int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  lastTime_ms = millis();
}

/**
 * the main loop
 */
void vj_loop() {
  unsigned long currentTime_ms = millis();
  unsigned long timeDiff_ms = currentTime_ms - lastTime_ms;
  // get microphone amplitude map it to something we can use for neopixels
  int micSignalAmplitude = getMicVolume(micPin, numMicSamples);

  float averageAmplitude = getAverageAmplitude(micSignalAmplitude, volumeHistory);
  
  byte brightness = map(micSignalAmplitude, 0, 1023, 0, 254);

  drawPixels(strip, brightness, currentTime_ms); 
  delay(5);

}
