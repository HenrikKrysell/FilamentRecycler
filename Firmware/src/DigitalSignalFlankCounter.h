#ifndef DigitalSignalFlankCounter_h
#define DigitalSignalFlankCounter_h

#include <Arduino.h>

class DigitalSignalFlankCounter
{
  public:
    void init(int digitalPin, unsigned long pollIntervalMs, unsigned long delayBeforeFirstPollMs) {
        _digitalPin = digitalPin;
        _pollIntervalMs = pollIntervalMs;

        // delayBeforeFirstPollUs must be less than pollIntervalUs
        if (delayBeforeFirstPollMs > pollIntervalMs)
            delayBeforeFirstPollMs = pollIntervalMs;

        lastValue = 0;
        count = 0;
        _lastPollTime = millis() - pollIntervalMs + delayBeforeFirstPollMs;
        pinMode(_digitalPin, INPUT);
    }

    void run() {
        if ((millis() - _lastPollTime >= _pollIntervalMs)) {
          _lastPollTime = millis();
          int value = digitalRead(_digitalPin);
          count += value != lastValue ? 1 : 0;
          lastValue = value;
      }
    }

    void resetCount() { count = 0; }
    
    int getCount() { return count; }

  private:

    int _digitalPin;
    unsigned long _pollIntervalMs;
    unsigned long _lastPollTime;
    int lastValue;
    int count;
};

#endif