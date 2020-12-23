

class Plotter {
  public:
    Plotter();
    ~Plotter();
    void setPins();
    void moveTo(float xPos, float yPos);
    void moveBy(float y, float x);
  private:
    uint8_t
      _xPos,
      _yPos,
      _headRotation;
    boolean
      _zHeadUp,
      _grabberActive;
    
}
