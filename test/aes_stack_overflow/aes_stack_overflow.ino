// http://stackoverflow.com/a/33185921
#include "AES.h"

AES aes ;

//2b7e151628aed2a6abf7158809cf4f3c
byte key[] = {
  0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c, 
};

//6bc1bee22e409f96e93d7e117393172a
byte plain[] = {
  0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a, 
};

//000102030405060708090A0B0C0D0E0F
byte my_iv[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
};

byte encrypted[] = {
  0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
};
byte cipher [N_BLOCK] ;
byte check [N_BLOCK] ;

void loop () 
{
}

void setup () 
{
  Serial.begin (115200) ;

  while (!Serial) ;
  Serial.println ("Ready") ;

  Serial.print("N_BLOCK: ") ;
  Serial.println (N_BLOCK) ;

  prekey (128, 2) ;
}

void prekey (int bits, int blocks)
{
  byte iv [N_BLOCK] ;

  long t0 = micros () ;
  byte succ = aes.set_key (key, bits) ;
  long t1 = micros()-t0 ;
  Serial.print ("set_key ") ; Serial.print (bits) ; Serial.print (" ->") ; Serial.print ((int) succ) ;
  Serial.print (" took ") ; Serial.print (t1) ; Serial.println ("us") ;
  t0 = micros () ;
  if (blocks == 1)
    succ = aes.encrypt (plain, cipher) ;
  else
  {
    for (byte i = 0 ; i < 16 ; i++)
      iv[i] = my_iv[i] ;
    succ = aes.cbc_encrypt (plain, cipher, blocks, iv) ;
  }
  t1 = micros () - t0 ;
  Serial.print ("encrypt") ; Serial.print (" ->") ; Serial.print ((int) succ) ;
  Serial.print (" took ") ; Serial.print (t1) ; Serial.println ("us") ;
  Serial.println("done encrypting");
  
  t0 = micros () ;
  if (blocks == 1) {
    Serial.println("only one block");
    succ = aes.decrypt (cipher, plain) ;
  } else {
    Serial.println("more than one block");
    for (byte i = 0 ; i < 16 ; i++) {
      iv[i] = my_iv[i] ;
    }
    Serial.println("done copying initialization vector");
    //succ = aes.cbc_decrypt (cipher, check, blocks, iv) ;
    
    succ = aes.cbc_decrypt (encrypted, check, blocks, iv) ;
    Serial.println("done decrypting");
  }
  t1 = micros () - t0 ;
  Serial.print ("decrypt") ; Serial.print (" ->") ; Serial.print ((int) succ) ;
  Serial.print (" took ") ; Serial.print (t1) ; Serial.println ("us") ;

  for (byte ph = 0 ; ph < 5 ; ph++)
  {
    Serial.print(ph);
    Serial.println(" hahah");
    Serial.print(ph == 0 ? "plain:  " : ph == 1 ? "key:    " : ph == 2 ? "iv:     " : ph == 3 ? "enc:    " : "dec:    ") ;
    for (byte i = 0 ; i < (blocks-1)*N_BLOCK ; i++)
    {
      byte val = ph == 0 ? plain[i] : ph == 1 ? key[i] : ph == 2 ? my_iv[i] : ph == 3 ? cipher[i] : check[i] ;
      Serial.print (val>>4, HEX) ; Serial.print (val&15, HEX) ;
    }
    Serial.println () ;
  }  
}
