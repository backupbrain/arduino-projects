
#include "Arduino.h"
#include "FunctionGenerator.h"

const unsigned int FunctionGenerator::BASELINE = 511;
const unsigned int FunctionGenerator::MAX_AMPLITUDE = 512;
    

FunctionGenerator::FunctionGenerator(unsigned int numFunctions) {
  this->MAX_FUNCTIONS = numFunctions;
  this->_functions = (Function*) malloc (numFunctions*sizeof(Function));
  this->_carotPosition = 0;
  
  Function* function;
  for (unsigned int index = 0; index < this->MAX_FUNCTIONS; index++) {
    function = this->getFunction(index);
    function->active = false;
  }
}
FunctionGenerator::~FunctionGenerator() {
  free(this->_functions);
}

unsigned int FunctionGenerator::addFunction(unsigned int amp, unsigned int frequency_hz, int phase_ms) {
  if (amp > this->MAX_AMPLITUDE) amp = this->MAX_AMPLITUDE;
  unsigned int retval = -1;
  if (this->_carotPosition < this->MAX_FUNCTIONS) {
    Function function;
    function.amplitude = amp;
    function.frequency_hz = frequency_hz;
    function.phase_ms = phase_ms;
    function.active = true;
    _functions[this->_carotPosition] = function;
    retval = this->_carotPosition;
    this->_carotPosition++;
  }
  return retval;
}
void FunctionGenerator::removeFunction(unsigned int index) {
  if (index < this->_carotPosition) {
    this->_functions[index].amplitude = 0;
    this->_functions[index].frequency_hz = 0;
    this->_functions[index].phase_ms = 0;
    this->_functions[index].active = false;
  }
}
Function* FunctionGenerator::getFunction(unsigned int index) {
  Function* function;
  if (index < this->MAX_FUNCTIONS) {
    function = this->_functions+(index*sizeof(Function));
  }
  return function;
}
unsigned int FunctionGenerator::getSnapshot() {
  unsigned int snapshot = 0;
  for (unsigned int index=0; index<this->MAX_FUNCTIONS; index++) {
    snapshot += this->_getFunctionSnapshot(index);
  }
  
  snapshot += this->BASELINE;
  
  //if (snapshot > this->MAX_AMPLITUDE*2) snapshot = this->MAX_AMPLITUDE*2;
  //if (snapshot < 0) snapshot = 0;
  return snapshot;
}
    
unsigned int FunctionGenerator::_getFunctionSnapshot(unsigned int index) {
  unsigned long time = millis();
  Function* function;
  float realSnapshot = 0;
  float rads = 0;
  unsigned int snapshot = 0;
  if (index < this->MAX_FUNCTIONS) {
    function = this->getFunction(index);
  }
 
  if (function->active) {
    float radsToTime = 2*PI;
    rads = (float)time/1000.0 + (function->frequency_hz * radsToTime) + function->phase_ms/1000;
    realSnapshot = sin(rads)*(function->amplitude/2);
  
    snapshot = (int)floor(realSnapshot);
  }
  return snapshot;
}
    
