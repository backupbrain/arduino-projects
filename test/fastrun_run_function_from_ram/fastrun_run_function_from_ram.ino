// FASTRUN example:
// taken from:
// https://forum.pjrc.com/threads/27690-IntervalTimer-is-not-precise?p=64142&viewfull=1#post64142

#define PIN  9   // output pin

int L = 3;   // length of sequence
boolean seq[3] = {1, 0, 1}; // sequence to be played
int Fs = 32768; // sampling rate

IntervalTimer t;

volatile int ind = 0;

FASTRUN void next_digit(){
  if (ind<L){
    digitalWriteFast(PIN, seq[ind]);
  }
  else if (ind<=Fs) {
    digitalWriteFast(PIN, 0);
  }
  else {
    ind=-1;
  }
  ind++;  
}

void setup() {
  SCB_SHPR3 = 0x20200000;  // Systick = priority 32 (defaults to zero)
  pinMode(PIN, OUTPUT);
  t.priority(0);
  t.begin(next_digit, 1000000.0/Fs);
  t.priority(0);
}
