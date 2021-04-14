#include "StepperHomingHelper.h"

const int A4988InterfaceType = 1;

StepperHomingHelper::StepperHomingHelper(AccelStepper* stepper, MotorDefinition motorDefinition, int endStopPin)
{
  _stepper = stepper;
  _motorDefinition = motorDefinition;
  _endStopPin = endStopPin;
}

bool StepperHomingHelper::start()
{
  _isHoming = true;
  _homingDone = false;
  _currentState = STATES.START;
}

bool StepperHomingHelper::stop()
{
  _isHoming = false;
  _homingDone = false;
}

bool StepperHomingHelper::loop()
{
  switch (_currentState)
  {
    case STATES.START:
    break;
    case STATES.EndStopTriggeredAtStart:
    break;

  }
  if (_isWinding)
  {
    int maxLimitValue = digitalRead(_maxLimitSwitchPin);
    if (maxLimitValue > 0)
      _isWinding = false;
  }
  else
  {
    int minLimitValue = digitalRead(_minLimitSwitchPin);
    if (minLimitValue > 0)
      _isWinding = true;
  }
    
  if (_isWinding)
    _stepper->runSpeed();
}

