/* Heartbeat2
 *   Paul Badger 2009
 *   Demonstates how to use an array with millis to achieve irregular rhythms
 *   This function has no delays but it may leave the LEDpin / LED in 
 *   the ON state, if the user stops calling it. Hence the "stopHeatbeat()" function
 */

long heartBeatArray[] = {
    50, 100, 15, 1200};
int hbeatIndex = 1;    // this initialization is not important  
long prevMillis;

int LEDpin = 13;

int fadeInIterator = 100;
int fadeOutIterator = 10;
int currentBrightness = 0;

void setup()                    
{
    Serial.begin(9600);
    pinMode(LEDpin, OUTPUT);
}

void loop()                    
{
    heartBeat(1.0);     // try changing parameter
}

void heartBeat(float tempo){
    if ((millis() - prevMillis) > (long)(heartBeatArray[hbeatIndex] * tempo)){
        hbeatIndex++;
        if (hbeatIndex > 3) hbeatIndex = 0;

        if ((hbeatIndex % 2) == 0){     // modulo 2 operator will be true on even counts
            currentBrightness += fadeInIterator;
            if (currentBrightness > 255) {
              currentBrightness = 255;
            }
            analogWrite(LEDpin, currentBrightness);
        }
        else{ 
            currentBrightness -= fadeOutIterator;
            if (currentBrightness < 0) {
              currentBrightness = 0;
            }
            analogWrite(LEDpin, currentBrightness);
        }
        //  Serial.println(hbeatIndex);
        prevMillis = millis();
    }
}


void stopHeatbeat(){
    digitalWrite(LEDpin, LOW); 
}
