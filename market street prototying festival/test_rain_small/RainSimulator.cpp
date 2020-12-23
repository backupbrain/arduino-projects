#include "RainSimulator.h"


RainSimulator::RainSimulator(int outputPin, int numPixels) {
  _saturation = 255;
  _value = 127;

  _ledStrip = new WS2812(numPixels); 
  _ledStrip->setOutput(outputPin);

  _rainDrops = new RainDrop[numPixels];
}



RainSimulator::~RainSimulator() {
  delete _ledStrip;
  _ledStrip = NULL;
  
  delete _rainDrops;
  _rainDrops = NULL;
}



void RainSimulator::update() {
  _cycle();
  
  for(int i = 0; i < _ledStrip->getNumLeds(); i++) {
      _ledStrip->set_crgb_at(i, _color);
  }
  
  _ledStrip->sync();
}


void RainSimulator::_makeItRain() {
  // randomly generate rain drop
  // in real life, we will know how long the strip is and therefore know how fast to move the LEDs
  // a rain drop falls at about 20 miles per hour (10 meters per second)

  // create new rain drop when the random number generates something within our propability
  int rainProbability = 20;
  int spread = random(0, 100);
  if (rainProbability < spread) {
    _rainDrops[0].active = true;
    _rainDrops[0].velocity = 0;
    _rainDrops[0].position = 0;
    _rainDrops[0].age_ms = 0;
  }
  
}

void RainSimulator::_moveRainDrops() {
  // iterate backwards through rain drops.
  // if rain is at terminal velocity, just move
  // if rain is not at terminal velocity, accelerate by 9.8m/s^2

  _rainDrops[_ledStrip->getNumLeds()-1].active = false; // clear last rain drop

  for (int i = _ledStrip->getNumLeds()-2; i >= 0; i--) {
    if (_rainDrops[i].active == true) {
      _moveRainDrop(i, i+1);
    }
  }
}

void RainSimulator::_moveRainDrop(int fromId, int toId) {
  _rainDrops[toId].active = _rainDrops[fromId].active;
  _rainDrops[toId].velocity = _rainDrops[fromId].velocity;
  _rainDrops[toId].position = _rainDrops[fromId].position;
  _rainDrops[toId].age_ms = _rainDrops[fromId].age_ms;
  _rainDrops[fromId].active = false; 
}

void RainSimulator::render() {
  _makeItRain();
  _moveRainDrops();

  cRGB colorOn;
  colorOn.SetHSV(_HUE, _SATURATION, 100);
  
  cRGB colorOff;
  colorOff.SetHSV(_HUE, _SATURATION, 0);
  
  for (int i = 0; i < _ledStrip->getNumLeds(); i++) {
    
    if (_rainDrops[i].active == true) {
      _ledStrip->set_crgb_at(i, colorOff);
    } else {
      _ledStrip->set_crgb_at(i, colorOn);
    }
  }
  
  _ledStrip->sync();
}

void RainSimulator::_cycle() {
  Serial.print("Setting color to: ");
  Serial.print(_hue);
  Serial.print(", ");
  Serial.print(_saturation);
  Serial.print(", ");
  Serial.print(_value);
  Serial.println();
  _color.SetHSV(_hue, _saturation, _value);
  
  _hue += _hueIncrementor;  
  if(_hue > 360) {
      _hue %= 360;
  }
}
