/**
 * BogoMips Calculation
 * Calculates approximate millions of null instructions the CPU can 
 * calculate in one second.
 * 
 * Based on Linux BogoMips calculation, designed to approximate processing
 * capalilities on various architectures, where Mhz ratings may be irrelevant,
 * for instance if the CPU is very good at parallelizing.
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-12-15
 */


void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Arduino BogoMips calculator");
  Serial.println("===========================");
  Serial.println("Calculates number of times Ardunio can calculate a null instruction.");
  Serial.println("Emulates Linux 'BogoMips' speed calculation.");
  Serial.println();
  
  Serial.print("Waiting for clock to start...");
  while (millis() <= 0);
  Serial.println("ok");

  
  unsigned long MHz = 1000000;
  unsigned long microSeconds = 1000000;
  
  long counterStart = 1000000;
  long counter = counterStart;
  unsigned long endTime_us = 0;
  unsigned int timeDiff_us = 0;
  unsigned long startTime_us = micros();

  
  Serial.print("Running loop...");
  
  while (counter-- >=0) {
    asm("nop;"); // nop command does not get optimized out by the compiler
  }
  endTime_us = micros();

  Serial.println("done");
  

  timeDiff_us = endTime_us - startTime_us;

  
  Serial.print("Time elapsed: ");
  printFloat((float)timeDiff_us/(float)microSeconds, 5);
  Serial.println(" seconds");

  // microSeconds and million cancel each other out
  double bogomips = (float)counterStart/(float)timeDiff_us;

  Serial.println();
  Serial.print("CPU Speed: ");
  float cpuMhz = F_CPU/MHz;
  Serial.print(cpuMhz); 
  Serial.println("MHz");
  
  Serial.print("BogoMips: ");
  printFloat(bogomips, 10); 
  Serial.println();

  Serial.print("Rating: clock * ");
  float rating = bogomips / cpuMhz;
  printFloat(rating ,2);
  Serial.println();

  float intel386dxRatingIndex = 0.18;
  Serial.print("i386 rating comparison index: ");
  float ratingIndex = rating / intel386dxRatingIndex;
  printFloat(ratingIndex, 2);
  Serial.println();

  Serial.println("===========================");
  Serial.println("For more information about BogoMIPS, see: https://en.wikipedia.org/wiki/BogoMips");
  
  
}


void loop() {
  
}


// http://playground.arduino.cc/Code/PrintFloats
void printFloat(float value, int places) {
  // this is used to cast digits 
  int digit;
  float tens = 0.1;
  int tenscount = 0;
  int i;
  float tempfloat = value;

    // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
  // if this rounding step isn't here, the value  54.321 prints as 54.3209

  // calculate rounding term d:   0.5/pow(10,places)  
  float d = 0.5;
  if (value < 0)
    d *= -1.0;
  // divide by ten for each decimal place
  for (i = 0; i < places; i++)
    d/= 10.0;    
  // this small addition, combined with truncation will round our values properly 
  tempfloat +=  d;

  // first get value tens to be the large power of ten less than value
  // tenscount isn't necessary but it would be useful if you wanted to know after this how many chars the number will take

  if (value < 0)
    tempfloat *= -1.0;
  while ((tens * 10.0) <= tempfloat) {
    tens *= 10.0;
    tenscount += 1;
  }


  // write out the negative if needed
  if (value < 0)
    Serial.print('-');

  if (tenscount == 0)
    Serial.print(0, DEC);

  for (i=0; i< tenscount; i++) {
    digit = (int) (tempfloat/tens);
    Serial.print(digit, DEC);
    tempfloat = tempfloat - ((float)digit * tens);
    tens /= 10.0;
  }

  // if no places after decimal, stop now and return
  if (places <= 0)
    return;

  // otherwise, write the point and continue on
  Serial.print('.');  

  // now write out each decimal place by shifting digits one by one into the ones place and writing the truncated value
  for (i = 0; i < places; i++) {
    tempfloat *= 10.0; 
    digit = (int) tempfloat;
    Serial.print(digit,DEC);  
    // once written, subtract off that digit
    tempfloat = tempfloat - (float) digit; 
  }
}

