
#define BIT_RESOLUTION 12
#define ACD_PIN A0
#define DAC_PIN 11
#define ADC_3V_PIN 10

//#define ACD_PIN A2
//#define DAC_PIN A14

int sample = 0;
int incoming_3v = 0;
unsigned long start_time = 0;
unsigned long current_time = 0;
unsigned long time_offset = 0;


void setup() {
  Serial.begin(9600);
  pinMode(DAC_PIN, OUTPUT);
  //analogWriteResolution(BIT_RESOLUTION);  // set the analog output resolution to 12 bit (4096 levels)
  //analogReadResolution(BIT_RESOLUTION);   // set the analog input resolution to 12 bit 
  
  start_time = millis();
}

void loop() {
    
  
  current_time = millis();
  time_offset = current_time - start_time;
  double sineValue = getSine(time_offset);
  int bitValue = sineToByteValue(sineValue);
  
  
  // write the selected waveform on DAC0
  analogWrite(DAC_PIN, bitValue);  
  
  //sample = map(analogRead(A0), 0, 4095, 0, oneHzSample);
  sample = analogRead(ACD_PIN);
  
  incoming_3v = analogRead(ADC_3V_PIN);
  
  
  Serial.print(bitValue);
  Serial.print(":");
  Serial.print(sample);
  Serial.print(":");
  Serial.print(incoming_3v);
  Serial.print("\n");
  
  delay(100);
  
}

/**
 * converts time_offset in milliseconds to a value between -1 and 1,
 * with a periaod duration of 1 second
 */
double getSine(unsigned long time_offset) {
  float seconds = time_offset / 10;
  float rads = seconds * 2 * 3.14;
  double sineValue = sin(rads);
  return sineValue;
}

/**
 * converts value -1<x<1 to 0<x<255 
 */
byte sineToByteValue(double sineValue) {
  float byteValue = (sineValue+1)*(1024/2);
  return (int) byteValue;
}
