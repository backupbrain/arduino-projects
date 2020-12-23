/**
 * for display, memory, and others, need SPI and i2C
 * for now, probably best to have a monolithic kernel, as I have no idea how to load modules
 *
 * things to think about - 
 * - step 1: get keyboard input from Serial buffer
 * - step 2; display output through serial, so we can create a display emulator in Processing
 * - Step 3: keyboard interrupt controller?
 * - Step 4: filesystem?
 * - Step 5: module loading?
 */

#include "KeyboardSettings.h"
#include "Commands.h"

String incomingcommand = "";
char character;
char newline = '\n';

void setup() {
	Serial.begin(9600);
        // here we need to mount the filesystem

        // we should also connect Networking here.
}

void loop() {
	
        // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte, DEC);


                character = Serial.read();


                if (strcmp(character, newline) == 0) {
                        // parse command
                        parseCommand(incomingcommand);
                        // reset command string
                        incomingcommand = "";

                } else {
                        incomingcommand.concat(character);
                }
        }



        // in this operating system, when running programs, 
        // we need to be always, always checking for available memory
        // no memory to run a program? PROBLEM!
        // no memory to open a file? PROBLEM!
}


// in lieu of actual programs, we can take in strings and treat them like commands.
void parseCommand(String incomingcommand) {
        // split by spaces
        char command[] = 'ls';
        char parameter[] = 'test.txt';
        // look for matching command in first string
        if (strcmp(command, CMD_DIRECTORY_LIST) == 0) {

        }
        if (strcmp(command, CMD_READ_FILE) == 0) {
                // find the file and echo it to the screen

        }
        if (strcmp(command, CMD_BLINK) == 0) {
                runProgram();
        }

}

void runProgram() {
        // inline assembler taken from 
        // https://www.youtube.com/watch?v=TOqnijfi9eI

        // the goal here is to test running assembly code from a string
        // in preparation for opening a string from the filesystem and running
        // it as a program
        char portableExecutable = "
sbi 0x04, 5
rcall   start

start:
sbi     0x05,   5
ldi     r16,    40
rcall   delay
cbi     0x05,   5
ldi     r16,    40
rcall   delay
rjmp    start

delay:
dec     r8
brne    delay
dec     r9
brne    delay
dec     16
brne    delay
ret
        "
        asm(portableExecutable);



}