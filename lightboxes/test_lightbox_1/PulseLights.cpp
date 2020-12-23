/**
   PulseLights.cpp
*/

#include "PulseLights.h"

PulseLights::PulseLights() {
  _bpm = DEFAULT_BPM;
  _initializePulseLights();
}

PulseLights::~PulseLights() {
}

void PulseLights::_initializePulseLights() {
  _lastRenderTime_ms = millis();
  _brightness = 0;
  setBeatsPerMinute(DEFAULT_BPM);
}


void PulseLights::setBeatsPerMinute(const unsigned int bpm) {
  _bpm = bpm;
  
  _timeBetweenBeats_ms = round(1000.0 * 60.0 / (float) _bpm);
  //long nextBeatTime_ms = _lastBeatTime_ms + timeBetweenBeats_ms;
  _timeBetweenSecondaryBeats_ms = round(_timeBetweenBeats_ms / 3.0);
}

unsigned int PulseLights::getBeatsPerMinute() {
  return _bpm;
}

int PulseLights::render() {
  long currentTime_ms = millis();
  long timeSinceLastRender_ms = currentTime_ms - _lastRenderTime_ms;

  if (_isBeating == true) {
    int dimmingAmount = ceil((_timeBetweenBeats_ms / 20) * timeSinceLastRender_ms / (float) _BEAT_TIMEOUT_MS);
    _brightness += dimmingAmount; //50;
    if (_brightness > 255) {
      _brightness = 255;
      _isBeating = false;
    }
  } else {
    int dimmingAmount = ceil((_timeBetweenBeats_ms / 20) * timeSinceLastRender_ms / (float) _FADE_TIMEOUT_MS);
    _brightness -= dimmingAmount; //10;
    if (_brightness < 0) {
      _brightness = 0;
    }
  }

  long timeSinceLastBeat_ms = currentTime_ms - _lastBeatTime_ms;
  if ((_didFirstBeatPast == false) && (timeSinceLastBeat_ms > _timeBetweenSecondaryBeats_ms)) {
    _didFirstBeatPast = true;
    _isBeating = true;
  }
  if (timeSinceLastBeat_ms > _timeBetweenBeats_ms) {
    _didFirstBeatPast = false;
    _isBeating = true;
    _lastBeatTime_ms = currentTime_ms;
  }
  
  _lastRenderTime_ms = currentTime_ms;
  return _brightness;
}

