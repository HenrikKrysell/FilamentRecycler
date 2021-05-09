#ifndef Potentiometer_h
#define Potentiometer_h

#include <Arduino.h>
#include "../Utilities/IntervalTimer.h"

class Potentiometer
{
  public:
    Potentiometer(int potentiometerPin, int pollIntervalMs);
    ~Potentiometer();
    
    void setup();
    int fetchPosition();

  private:
    int _potentiometerPin;
    int _currentValue;
    IntervalTimer _intervalTimer;
};

#endif