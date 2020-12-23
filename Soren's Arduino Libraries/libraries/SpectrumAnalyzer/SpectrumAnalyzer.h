// COPYRIGHT ZACH VORHIES 2012.

#ifndef SPECTRUM_ANALYZER_H_
#define SPECTRUM_ANALYZER_H_

#include <Arduino.h>

// EXAMPLE:
//  SpectrumAnalyzer SpectrumAnalyzer;  // Use default shield pins.
//  void setup() {
//    // No setup necessary for SpectrumAnalyzer.
//    Serial.begin(9600);  // For SpectrumAnalyzer::PrintBands(...).
//  }
//  void loop() {
//    SpectrumAnalyzer::Output freq_output;
//    SpectrumAnalyzer.ProcessAudioTo(&freq_output);
//    SpectrumAnalyzer.PrintBands(freq_output);
//  }
//
// NOTE: This object will modify global state! Specifically
// Instantiating this class will set the analogPin's reference
// voltage to the DEFAULT setting. This will be 5V on Arduino 5V
// boards and 3.3 volts on 3.3V Arduino boards. See SpectrumAnalyzer.cc
// for specifics.
class SpectrumAnalyzer {
 public:

  struct Output {
    // spectrum_array[0] => lowest frequency.
    // spectrum_array[spectrum_array_size-1] => highest frequency.
    int spectrum_array[7];
    static const int spectrum_array_size = 7;
    Output() : spectrum_array() {}
  };

  // Constructs the SpectrumAnalyzer driver with the default analog, reset and strobe
  // pins such that analogPin = 0, resetPin = 7, strobePin = 8.
  SpectrumAnalyzer();
  ~SpectrumAnalyzer() {}
  SpectrumAnalyzer(int left_analog_pin, int right_analog_pin, int resetPin, int strobePin);

  // Process the audio and stores the result in the output structure.
  void ProcessAudio(Output* left_channel, Output* right_channel);
  // Prints the bands out. This is useful for debugging purposes.
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

  void Init(int left_channel_pin, int right_channel_pin, int resetPin, int strobePin);
  void ProcessAudioChannel(int analog_pin, Output* out);

  int left_channel_pin_;
  int right_channel_pin_;
  int reset_pin_;
  int strobe_pin_;
  int filter_value_;
  int gain_;
};

#endif  // SPECTRUM_ANALYZER_H_
