#ifndef RAINSIMULATOR_H
#define RAINSIMULATOR_H

#include "WS2812.h"

struct RainDrop {
  boolean active;
  int position;
  int velocity;
  int age_ms;
  
};

class RainSimulator {
  public:
    RainSimulator(int outputPin, int numPixels);
    ~RainSimulator();

    void update();
    void render();
    
  private:
    const byte _HUE = 203; // purple
    const byte _SATURATION = 92; // full saturation
    const int _hueIncrementor = 1;
    const long animationTimeout_ms = 100; //delays between update
    WS2812* _ledStrip;

    RainDrop* _rainDrops;


    void _cycle();
    void _makeItRain();
    void _moveRainDrops();
    void _moveRainDrop(int fromId, int toId);

    void _addRaindrop();
    void _removeRaindrop();
    cRGB _color;
    int _hue;
    byte _saturation;
    byte _value;
    
};


#endif /* RAINSIMULATOR_H */
