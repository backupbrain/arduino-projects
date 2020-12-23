

#ifndef VirtualMachine_h
#define VirtualMachine_h
  
class VirtualMachine {
  public:
    VirtualMachine();
    void clear(byte n);
    void debug(byte n);
    void init(void);
    void load_prog(void);
    void init_prog(byte n);
    void serial_begin(void);
    void run(byte n);
    void reset(void);
 private:
    static const _ROM_SIZE = 2048;
    static const _STACK_SIZE = 512;
    static const _COMMON = 64;

    byte _N,
        _i,
        _temp = 1;
    byte _opcode,
        _operand;
    int _count = 0,
        _cam_mem_p = 0;
    int _com_mem[this->COMMON];
};


#endif
