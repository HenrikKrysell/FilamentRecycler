#include "StepperHomingHelper.h"

StepperHomingHelper::StepperHomingHelper(AccelStepper* stepper, MotorDefinition motorDefinition, int endStopPin)
{
  _stepper = stepper;
  _motorDefinition = motorDefinition;
  _endStopPin = endStopPin;
}

void StepperHomingHelper::start()
{
  _currentState = STATES::Start;
}

bool StepperHomingHelper::loop()
{
  switch (_currentState)
  {
    case STATES::Start:
    {
      if (digitalRead(_endStopPin) > 0)
        _currentState = STATES::EndStopTriggeredAtStart;
      else
        _currentState = STATES::MoveTowardsEndStopFast;
    }
    break;
    case STATES::EndStopTriggeredAtStart:
    {
      _stepper->setSpeed(-1000);
      _stepper->runSpeed();
      if (digitalRead(_endStopPin) <= 0)
        _currentState = STATES::MoveTowardsEndStopFast;
    }
    break;
    case STATES::MoveTowardsEndStopFast:
    {
      _stepper->setSpeed(1000);
      _stepper->runSpeed();
      if (digitalRead(_endStopPin) > 0)
        _currentState = STATES::EndStopTriggeredFirstTime;
    }
    break;
    case STATES::EndStopTriggeredFirstTime:
    {
      _stepper->setSpeed(-100);
      _stepper->runSpeed();
      if (digitalRead(_endStopPin) <= 0)
        _currentState = STATES::MoveTowardsEndStopSlow;
    }
    break;
    case STATES::MoveTowardsEndStopSlow:
    {
      _stepper->setSpeed(100);
      _stepper->runSpeed();
      if (digitalRead(_endStopPin) > 0)
      {
        _stepper->setCurrentPosition(_stepper->currentPosition());
        _currentState = STATES::Done;
      }
    }
    break;
  }

  return _currentState == STATES::Done;
}

