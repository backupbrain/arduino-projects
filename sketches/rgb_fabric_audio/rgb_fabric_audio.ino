byte RED_PIN = 11;
byte GREEN_PIN = 10;
byte BLUE_PIN = 9;

byte DELAY_TIME = 20;

int hue = 0;
byte saturation = 255;
byte value = 255;

byte red = 0;
byte green = 0;
byte blue = 0;

int hue_increment = 1;

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  
   Serial.begin(9600);
  
}


void loop() {
  
  
  
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(A0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
      
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 3.3) / 1024;  // convert to volts
 
   Serial.println(peakToPeak);
   
   value = (255*peakToPeak/1024);
  if (value > 255) value = 255;
  if (value < 0) value = 0;
  
  
  //delay(DELAY_TIME);
  hue++;
  HSV2RGB(red, green, blue, hue, saturation, value);
  /*
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
  */
}





void HSV2RGB(byte &R, byte &G, byte &B, int H, byte S, byte V) {
  float saturation = 255 - S;
  byte r = V * constrain(0.5+cos(radians(H)),0,1);
  byte g = V * constrain(0.5+cos(radians(H-120)),0,1);
  byte b = V * constrain(0.5+cos(radians(H+120)),0,1);
  byte white = 0.3*r + 0.59*g + 0.11*b;
  R = r + saturation/255 * (white - r);
  G = g + saturation/255 * (white - g);
  B = b + saturation/255 * (white - b);
}


