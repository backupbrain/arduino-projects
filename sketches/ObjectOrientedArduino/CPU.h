/**
 * This class represents an Arduino CPU
 * 
 * It is a singleton (you cannot create more than one instance) because
 * an Arduino only has one CPU.
 * 
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-01-28
 */
#ifndef CPU_h
#define CPU_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif


class CPU {
  private:
    CPU (void); // singleton class.
    // Copy and assignment is prohibted.
    CPU & operator=(const CPU &);
    static CPU* cpuInstance;
  
  public:
    CPU (const CPU &);
    ~CPU (void);
    static const CPU & Instance();
    static void Terminate();
    
    const void myFunc() { }
};

#endif 
