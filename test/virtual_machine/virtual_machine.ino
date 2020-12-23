/************************************************************
  Author :                      Nakul Rao I
  Date began :                  26/7/2013
  Date completed :              1/8/2013
  Instruction set verified on : 3/8/2013
 ************************************************************/
// taken from: https://nakulrao.wordpress.com/2013/08/03/virtual-machine-for-arduino/

 #include <stdint.h>
 #include <avr/interrupt.h>
 #include <avr/io.h>
 #include <avr/pgmspace.h>

#include "VirtualMachine.h"
  
  Virtual_Machine VM[2];      // Declare 2 virtual machines
 
 /****************************************************************************************************************************/
 
 void setup(){
   // begin setup
  pinMode(48, INPUT_PULLUP);      // VM prog mode
  pinMode(49, INPUT_PULLUP);      // VM reset
  
  DDRA = 0x00;      // Input
  DDRB = 0x00;      // Input
  DDRC = 0xFF;      // Output
  DDRD = 0xFF;      // Output

  serial_begin();  
    
  if((N > 2) || (N < 1)){
     Serial.println("VM aborted");
     Serial.println("Reset to start again");
     while(1);
  }
 
   VM_init(); 
  
   // Program VM
   init_prog(N);
   
   Serial.print("Beginning execution in 1 second");
   delay(1000);
 }                              // setup completed

 /****************************************************************************************************************************************/
 
 void loop(){
   if (digitalRead(48) == LOW){
     load_prog();
   }
   
   if (digitalRead(49) == LOW){
     reset_VM();
   }
   
   // Run VM1
   if ((VM[0].done == 0)){
     run(0);
   }
      
   // Run VM2
   if((N > 1) && (VM[1].done == 0)){   
     run(1);
   }     

 }

/*****************************************************************************************************************************************/


 
