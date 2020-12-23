// COPYRIGHT ZACH VORHIES 2012.

#include <SpectrumAnalyzer.h>

SpectrumAnalyzer::SpectrumAnalyzer() {
  int left_channel_pin = 0;
  int right_channel_pin = 1;
  int resetPin = 5;
  int strobePin = 4;
  Init(left_channel_pin, right_channel_pin, resetPin, strobePin);
}

SpectrumAnalyzer::SpectrumAnalyzer(int left_channel_pin, int right_channel_pin,
                                   int resetPin, int strobePin) {
  Init(left_channel_pin, right_channel_pin, resetPin, strobePin);
}

void SpectrumAnalyzer::ProcessAudio(Output* left, Output* right) {
  ProcessAudioChannel(left_channel_pin_, left);
  ProcessAudioChannel(right_channel_pin_, right);
}

void SpectrumAnalyzer::PrintBands(const Output& process_output) {
  const int* spectrum_array = process_output.spectrum_array;

  int anyPeaks = 0;
  for (int i = 0; i < 7; i++) {
    anyPeaks = anyPeaks || spectrum_array[i] > 1015;

    int val = spectrum_array[i];
    if (val < 10) {
      Serial.print("  ");
    } else if (val < 100) {
      Serial.print(" ");
    }
    Serial.print(val);
    Serial.print(" ");

    digitalWrite(strobe_pin_, HIGH);
  }

  if (anyPeaks) {
    Serial.print(" PEAK");
  }
  Serial.println();
}

void SpectrumAnalyzer::Init(int left_channel_pin, int right_channel_pin,
                            int resetPin, int strobePin) {
  gain_ = static_cast<int>(GainNormalization());
  filter_value_ = 90;

  reset_pin_ = resetPin;
  strobe_pin_ = strobePin;
  left_channel_pin_ = left_channel_pin;
  right_channel_pin_ = right_channel_pin;

  Serial.begin(9600);
  pinMode(left_channel_pin_, INPUT);
  pinMode(right_channel_pin_, INPUT);
  pinMode(strobe_pin_, OUTPUT);
  pinMode(reset_pin_, OUTPUT);

  // Set analogPin's reference voltage
  analogReference(DEFAULT); // 5V

  // Set startup values for pins
  digitalWrite(reset_pin_, LOW);
  digitalWrite(strobe_pin_, HIGH);
}


void SpectrumAnalyzer::ProcessAudioChannel(int analog_pin, Output* output) {
  static const int kFilterValue = 90;
  digitalWrite(reset_pin_, HIGH);
  digitalWrite(reset_pin_, LOW);

  int* spectrum_array = output->spectrum_array;

  int anyPeaks = 0;
  for (int i = 0; i < 7; i++) {

    digitalWrite(strobe_pin_, LOW);
    delayMicroseconds(30);

    // Average value of two reads.
    int newValue = (analogRead(analog_pin) + analogRead(analog_pin)) >> 1;

    newValue = constrain(newValue, kFilterValue, 1023);
    if (newValue < 25) {
      newValue = 0;
    }

    newValue = map(newValue, filter_value_, 1023, 0, 255);

    // gain_ scales the sound intensity. gain_ is in integer format for
    // efficiency on cpus which lack an FPU where 255 represents 1.0f.
    if (gain_ != GainNormalization()) {
      long gainValued = static_cast<long>(newValue) * static_cast<long>(gain_);
      gainValued = gainValued / GainNormalization();

      if (gainValued > 255)
        gainValued = 255;
      if (gainValued < 0)
        gainValued = 0;

      newValue = static_cast<int>(gainValued);
    }


    spectrum_array[i] = newValue;
    digitalWrite(strobe_pin_, HIGH);
  }
}
