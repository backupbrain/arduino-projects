// COPYRIGHT ZACH VORHIES 2012.

#ifndef SINESEZA_H_
#define SINESEZA_H_

#include <Arduino.h>

// EXAMPLE:
//  Sineseza sineseza;  // Use default shield pins.
//  void setup() {
//    // No setup necessary for Sineseza.
//    Serial.begin(9600);  // For Sineseza::PrintBands(...).
//  }
//  void loop() {
//    Sineseza::Output freq_output;
//    sineseza.ProcessAudioTo(&freq_output);
//    sineseza.PrintBands(freq_output);
//  }
//
// NOTE: This object will modify global state! Specifically
// Instantiating this class will set the analogPin's reference
// voltage to the DEFAULT setting. This will be 5V on Arduino 5V
// boards and 3.3 volts on 3.3V Arduino boards. See Sineseza.cc
// for specifics.
class Sineseza {
 public:

  struct Output {
    // spectrum_array[0] => lowest frequency.
    // spectrum_array[spectrum_array_size-1] => highest frequency.
    int spectrum_array[7];
    static const int spectrum_array_size = 7;
    Output() : spectrum_array() {}
  };

  // Constructs the Sineseza driver with the default analog, reset and strobe
  // pins such that analogPin = 0, resetPin = 7, strobePin = 8.
  Sineseza();
  ~Sineseza() {}
  Sineseza(int analogPin, int resetPin, int strobePin);

  // Process the audio and stores the result in the output structure.
  void ProcessAudioTo(Output* output);
  // Prints the pands out. This is usefull for debugging purposes.
  // Note that before you call this function you must setup serial
  // debugging; e.g. Serial.begin(9600).
  void PrintBands(const Output& process_output);

  void set_gain(float gain) {
    // gain is converted into integer format for better performance.
    gain_ = static_cast<int>(gain * GainNormalizationf());
  }

  float gain() const { return static_cast<float>(gain_) / GainNormalizationf(); }

 private:
  static float GainNormalizationf() { return 1024.0f; }
  static int GainNormalization() { return 1024; }

  void Init(int analogPin, int resetPin, int strobePin);

  int analog_pin_;
  int reset_pin_;
  int strobe_pin_;
  int filter_value_;
  int gain_;
};

#endif  // SINESEZA_H_
