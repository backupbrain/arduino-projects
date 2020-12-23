

class Plotter {
  public:
    Plotter();
    ~Plotter();
    void setPins(uint8_t xPositiveStepperPin, uint8_t xNegativeStepperPin, uint8_t yPositiveStepperPin, uint8_t yNegativeStepperPin, uint8_t zPinServoPin, uint8_t _rotationServoPin, uint8_t grabberPin);
    void setScale(unsigned long mmPerRotation_x, unsigned long mmPerRotation_y);
    void moveTo(float x_mm, float y_mm);
    void moveBy(float x_mm, float y_mm);
  private:
    unsigned long 
      _millimetersPerRotation_x,
      _millimetersPerRotation_y;
    
    uint8_t
      _xPositiveStepperPin,
      _xNegativeStepperPin,
      _yPositiveStepperPin,
      _yNegativeStepperPin,
      _grabberPin,
      _rotationServoPin,
      _zServoPin;
    
}
