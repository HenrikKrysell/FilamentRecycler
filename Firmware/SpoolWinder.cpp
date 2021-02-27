#include "SpoolWinder.h"

const int A4988InterfaceType = 1;


SpoolWinder::SpoolWinder(int motorStepPin, int motorDirPin, int motorEnablePin, int maxLimitSwitchPin, int minLimitSwitchPin)
{
  _motorStepPin = motorStepPin;
  _motorDirPin = motorDirPin;
  _motorEnablePin = motorEnablePin;
  _maxLimitSwitchPin = maxLimitSwitchPin;
  _minLimitSwitchPin = minLimitSwitchPin;
  _stepper = new AccelStepper(A4988InterfaceType, motorStepPin, motorDirPin);
  _isRunning = false;
}

void SpoolWinder::setup()
{
  pinMode(_minLimitSwitchPin, INPUT);
  pinMode(_maxLimitSwitchPin, INPUT);

  pinMode(_motorEnablePin, OUTPUT);
  digitalWrite(_motorEnablePin, LOW);

  // Set the maximum speed in steps per second:
  _stepper->setMaxSpeed(1000);
  _stepper->setSpeed(900);
}

void SpoolWinder::loop()
{
  if (_isRunning)
  {
    int maxLimitValue = digitalRead(_maxLimitSwitchPin);
    if (maxLimitValue > 0)
      _isRunning = false;
  }
  else
  {
    int minLimitValue = digitalRead(_minLimitSwitchPin);
    if (minLimitValue > 0)
      _isRunning = true;
  }
    
  if (_isRunning)
    _stepper->runSpeed();
}