#ifndef IntervalTimer_h
#define IntervalTimer_h

#include <Arduino.h>

class IntervalTimer
{
  public:
    IntervalTimer(int intervalMs=100)
    {
      _intervalMs = intervalMs;
      _lastReadTimestamp = 0;
    };

    void setInterval(int intervalMs)
    {
      _intervalMs = intervalMs;
    };
    
    inline bool isTriggered() __attribute__((always_inline))
    {
        unsigned long currentMillis = millis();
        if (currentMillis - _lastReadTimestamp > _intervalMs)
        {
          _lastReadTimestamp = currentMillis;
          return true;
        }

        return false;
    };

  private:
    unsigned long _lastReadTimestamp;
    int _intervalMs;
};

#endif