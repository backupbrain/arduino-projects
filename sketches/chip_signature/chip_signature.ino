//#include <avr/boot.h>
/*
#if defined(__AVR_mk20x256__)
static String board = "teensy31";
#elif defined(__AVR_mk20dx128__)
static String board = "teensy30";
#elif defined(__AVR_mkl26z64__)
static String board = "teensyLC";
#elif defined(__AVR_atmega32u4__)
static String board = "teensy2";
#elif defined(__AVR_ARCv2EM__)
static String board = "curie";
#elif defined(__AVR_esp8266__)
static String board = "esp8266";
#elif defined(__AVR_cortex_m0__)
static String board = "nrf51822";
#endif

#if defined(__AVR__)
static const unsigned int signature0 = SIGNATURE_0;
#endif
/* */
  

#if defined(__AVR_ATmega328P__) // uno, fio
static const int test = 0;
#else
static const int test = 1;
#endif


void setup() {
  Serial.begin(9600);
  
  while(!Serial);
  
  //Serial.println(bool_signature_byte_get(0x00);
  //Serial.println(SIGNATURE_0, HEX);
}

void loop() {
}
