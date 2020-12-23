// COPYRIGHT ZACH VORHIES 2012.

#include <Sineseza.h>

Sineseza::Sineseza() {
  int analogPin = 0;
  int resetPin = 7;
  int strobePin = 8;
  Init(analogPin, resetPin, strobePin);
}

Sineseza::Sineseza(int analogPin, int resetPin, int strobePin) {
  Init(analogPin, resetPin, strobePin);
}

void Sineseza::ProcessAudioTo(Output* output) {
  static const int kFilterValue = 90;
  digitalWrite(reset_pin_, HIGH);
  digitalWrite(reset_pin_, LOW);

  int* spectrum_array = output->spectrum_array;

  int anyPeaks = 0;
  for (int i = 0; i < 7; i++) {

    digitalWrite(strobe_pin_, LOW);
    delayMicroseconds(30);

    int newValue = analogRead(analog_pin_);

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

void Sineseza::PrintBands(const Output& process_output) {
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

void Sineseza::Init(int analogPin, int resetPin, int strobePin) {
  gain_ = static_cast<int>(GainNormalization());
  filter_value_ = 90;

  analog_pin_ = analogPin;
  reset_pin_ = resetPin;
  strobe_pin_ = strobePin;

  Serial.begin(9600);
  pinMode(analog_pin_, INPUT);
  pinMode(strobe_pin_, OUTPUT);
  pinMode(reset_pin_, OUTPUT);

  // Set analogPin's reference voltage
  analogReference(DEFAULT); // 5V

  // Set startup values for pins
  digitalWrite(reset_pin_, LOW);
  digitalWrite(strobe_pin_, HIGH);
}