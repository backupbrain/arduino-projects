#include "AES.h"

AES aes ;

static const int numBits = 128;
static const int numBlocks = 4;

byte key[32] = "123456789abcdefg";
byte initializationVector[33] = "000102030405060708090A0B0C0D0E0F";

char* plain = "This is a lengthy message, will it decode?";

byte encryptedMessage[4*N_BLOCK] = {0};
byte decryptedMessage[4*N_BLOCK] = {0};


void loop () {}


void encryptMessage(unsigned char* plain, const int numBlocks, byte* initializationVector) {
  byte volatileIV[N_BLOCK] ;
  for (byte i = 0 ; i < 16 ; i++) {
    volatileIV[i] = initializationVector[i] ;
  }

  aes.cbc_encrypt (plain, encryptedMessage, numBlocks, volatileIV);

}


void decryptMessage(unsigned char* encryptedMessage, const int numBlocks, byte* initializationVector) {
  byte volatileIV[N_BLOCK] ;
  for (byte i = 0 ; i < 16 ; i++) {
    volatileIV[i] = initializationVector[i] ;
  }
  
  aes.cbc_decrypt( encryptedMessage, decryptedMessage, numBlocks, volatileIV);
}

void setup () {
  Serial.begin (9600) ;
  while(!Serial) {;}
  Serial.println ("testng mode");
  
  aes.set_key (key, numBits) ;
  
  Serial.println((char*)plain);

  encryptMessage((unsigned char* )plain, numBlocks, initializationVector);
  
  decryptMessage(encryptedMessage, numBlocks, initializationVector);
  
  Serial.print("decrypted: ");
  Serial.println((char*) decryptedMessage);


  Serial.println("Done");
}

