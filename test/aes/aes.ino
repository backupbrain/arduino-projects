#include "AES.h"

AES aes ;

byte key[] = 
{
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
} ;

//unsigned char plain[] = {'h','e','l','l','o',' ', 't', 'h', 'e','r','e','\0'};
unsigned char plain[] = {'T','h','i','s',' ','i','s',' ','a',' ','l','e','n','g','t','h','y',' ','m','e','s','s','a','g','e','.',' ',' ','w','i','l','l',' ','i','t',' ','d','e','c','r','y','p','t','?','\0'};
/*
byte plain[] =
{
  0xf3, 0x44, 0x81, 0xec, 0x3c, 0xc6, 0x27, 0xba, 0xcd, 0x5d, 0xc3, 0xfb, 0x08, 0xf2, 0x73, 0xe6,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
} ;
*/


byte encrypted[] = {
  0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
};

byte my_iv[] = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
} ;

byte cipher [4*N_BLOCK] ;
byte check [4*N_BLOCK] ;

void loop () 
{}


void setup ()
{
  Serial.begin (9600) ;
  while(!Serial) {;}
  Serial.println ("testng mode") ;

  prekey_test () ;
  
//  otfly_test () ;
//  otfly_test256 () ;

  Serial.println("Done");
}

void prekey (int bits, int blocks)
{
  Serial.print(bits);
  Serial.println(" bit test");
  byte iv [N_BLOCK] ;
  
  long t0 = micros () ;
  byte succ = aes.set_key (key, bits) ;
  long t1 = micros()-t0 ;
  Serial.print ("set_key ") ; Serial.print (bits) ; Serial.print (" ->") ; Serial.print ((int) succ) ;
  Serial.print (" took ") ; Serial.print (t1) ; Serial.println ("us") ;
  Serial.print(" encrypting: ");
  Serial.println((char*)plain);
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
  Serial.print ("encrypt ") ; Serial.print ((int) succ) ;
  Serial.print (" took ") ; Serial.print (t1) ; Serial.println ("us") ;

  unsigned char decrypted[4*N_BLOCK] = {0};
  t0 = micros () ;
  if (blocks == 1) {
    succ = aes.decrypt (cipher, decrypted); //plain) ;
    Serial.println(succ);
  } else {
    for (byte i = 0 ; i < 16 ; i++)
      iv[i] = my_iv[i] ;
    //succ = aes.cbc_decrypt (cipher, check, blocks, iv) ;
    succ = aes.cbc_decrypt (cipher, decrypted, blocks, iv) ;
  }
  t1 = micros () - t0 ;
  Serial.print ("decrypt ") ; Serial.print ((int) succ) ;
  Serial.print (" took ") ; Serial.print (t1) ; Serial.println ("us") ;
  Serial.print("decrypted: ");
  Serial.println((char*) decrypted);

  for (byte ph = 0 ; ph < (blocks == 1 ? 3 : 4) ; ph++)
  {
    Serial.print(ph == 0 ? "plain:  " : ph == 1 ? "key:    " : ph == 2 ? "iv:     " : ph == 3 ? "enc:    " : "dec:    ") ;
    for (byte i = 0 ; i < (ph < 3 ? blocks*N_BLOCK : N_BLOCK) ; i++)
    {
      //byte val = ph == 0 ? plain[i] : ph == 1 ? cipher[i] : ph == 2 ? check[i] : iv[i] ;
      byte val = ph == 0 ? plain[i] : ph == 1 ? cipher[i] : ph == 2 ? decrypted[i] : iv[i] ;
      Serial.print (val>>4, HEX) ; Serial.print (val&15, HEX) ; Serial.print (" ") ;
    }
    Serial.println () ;
  }
  Serial.print(bits);
  Serial.println(" bit encrypt/decrypt complete");
}

void prekey_test ()
{
  prekey (128, 1) ;
  prekey (128, 4) ;
  Serial.println("Done with tests");
  /*
  prekey (192, 3) ;
  prekey (256, 2) ;
  prekey (128, 1) ;
  prekey (192, 1) ;
  prekey (256, 1) ;
  */
}


