

#ifndef FunctionGenerator_h
#define FunctionGenerator_h


struct Function {
  bool active;
  unsigned int amplitude;
  unsigned int frequency_hz; 
  int phase_ms;
};

class FunctionGenerator {
  public:
    unsigned int MAX_FUNCTIONS;
    static const unsigned int BASELINE;
    static const unsigned int MAX_AMPLITUDE;
    
    FunctionGenerator(unsigned int numFunctions);
    ~FunctionGenerator();

    // insert new function, return array position of this function
    // return -1 on failure
    unsigned int addFunction(unsigned int amp, unsigned int frequency_hz, int phase_ms);
    void removeFunction(unsigned int index);
    Function* getFunction(unsigned int index);
    unsigned int getSnapshot();
    
  private: 
    Function* _functions;
    unsigned int _carotPosition;
    unsigned int _getFunctionSnapshot(unsigned int index);
    
};
  
#endif
