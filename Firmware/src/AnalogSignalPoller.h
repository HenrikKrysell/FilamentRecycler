#ifndef AnalogSignalPoller_h
#define AnalogSignalPoller_h

#include <Arduino.h>

class AnalogSignalPoller
{
  public:
    void init(int analogPin, unsigned long pollIntervalMs, unsigned long delayBeforeFirstPollMs) {
        _analogPin = analogPin;
        _pollIntervalUs = pollIntervalMs;

        // delayBeforeFirstPollUs must be less than pollIntervalUs
        if (delayBeforeFirstPollMs > pollIntervalMs)
            delayBeforeFirstPollMs = pollIntervalMs;

        _lastPollTime = micros() - pollIntervalMs + delayBeforeFirstPollMs;
        pinMode(_analogPin, INPUT);
    }


    void run() {
        if ((millis() - _lastPollTime >= _pollIntervalUs)) {
          _lastPollTime = millis();
          value = analogRead(_analogPin);
      }
    }

    int getValue() { return value; }

  private:

    int _analogPin;
    unsigned long _pollIntervalUs;
    unsigned long _lastPollTime;
    int value;
};

#endif