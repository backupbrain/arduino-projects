
/**
 * PulseLights.h
 */

#ifndef PulseLights_h
#define PulseLights_h

#include <Arduino.h>

class PulseLights {
  public:
    static const unsigned int DEFAULT_BPM = 60;
    PulseLights();
    ~PulseLights();

    void setBeatsPerMinute(const unsigned int bpm);
    unsigned int getBeatsPerMinute();
    int render();

  private:
    static const unsigned int _MILLIS_TO_MINUTES;
    static const unsigned long _BEAT_TIMEOUT_MS = 30;
    static const unsigned long _FADE_TIMEOUT_MS = 200;
    unsigned long _lastRenderTime_ms;

    unsigned int _bpm;
    unsigned long _timeBetweenBeats_ms;
    unsigned long _timeBetweenSecondaryBeats_ms;
    
    bool _isBeating = false;
    bool _didFirstBeatPast = false;
    long _lastBeatTime_ms;
    int _brightness;

    void _initializePulseLights();
};

#endif
