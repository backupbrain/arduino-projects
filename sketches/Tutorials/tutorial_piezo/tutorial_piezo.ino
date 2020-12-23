const int tempIn = A0;

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

void setup() {
  pinMode(redPin, INPUT);
  pinMode(greenPin, INPUT);
  pinMode(bluePin, INPUT);
  Serial.begin(9600);
}

void loop() {
  
  int val = analogRead(tempIn);
  
  
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = val * 5.0;
 voltage /= 1024.0; 
 
 
 // print out the voltage
 Serial.print(val); Serial.println(" vals");
 Serial.print(voltage); Serial.println(" volts");
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((volatge - 500mV) times 100)
 Serial.print(temperatureC); Serial.println(" degrees C");
 
 // now convert to Fahrenheight
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 Serial.print(temperatureF); Serial.println(" degrees F");
  
  //outputValue = map(val, 0.5, 1023, 0, 255);
  
  
  
  int outputValue = map(val, 100, 150, 0, 255);
  setColor(outputValue, outputValue, outputValue);
  
  //Serial.println(tempIn);
  
  delay(1000);
}


void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
