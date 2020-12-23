#include "Adafruit_NeoPixel.h"
#include "isin.h"

//#define ORIG 1
#ifdef ORIG
  //#include "vj.h"
  #include "hacked.h"
#else
  #include "aj.h"
#endif 

void setup() {
  vj_setup();
}
void loop() {
  vj_loop();
}




