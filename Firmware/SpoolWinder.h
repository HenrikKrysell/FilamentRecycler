#ifndef SpoolWinder_h
#define SpoolWinder_h

#include <Arduino.h>
#include <AccelStepper.h>

class SpoolWinder
{
  public:
    SpoolWinder(int motorStepPin, int motorDirPin, int motorEnablePin, int maxLimitSwitchPin, int minLimitSwitchPin);
    void setup();
    void loop();

  private:
    int _motorStepPin;
    int _motorDirPin;
    int _motorEnablePin;
    int _maxLimitSwitchPin;
    int _minLimitSwitchPin;
    bool _isRunning;
    AccelStepper* _stepper;
};

#endif