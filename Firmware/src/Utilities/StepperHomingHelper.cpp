#include "StepperHomingHelper.h"

StepperHomingHelper::StepperHomingHelper(StepperMotor* stepper, MotorDefinition motorDefinition, int endStopPin)
{
  _stepper = stepper;
  _motorDefinition = motorDefinition;
  _endStopPin = endStopPin;
  pinMode(_endStopPin, INPUT_PULLUP);
}

void StepperHomingHelper::start()
{
  _currentState = STATES::Start;
}

void StepperHomingHelper::changeState(STATES newState)
{
  switch (newState)
  {
  case STATES::EndStopTriggeredAtStart:
    _stepper->setRPM(NORMAL_RPM);
    _stepper->startRunContinuous(BACKWARD);
    break;
  case STATES::MoveTowardsEndStopFast:
    _stepper->setRPM(NORMAL_RPM);
    _stepper->startRunContinuous(FORWARD);
    break;
  case STATES::EndStopTriggeredFirstTime:
    _stepper->setRPM(SLOW_RPM);
    _stepper->startRunContinuous(BACKWARD);
    break;
  case STATES::Backup1000StepsAfterEndStopTriggered:
    _stepper->setRPM(SLOW_RPM);
    _stepper->setRelativeTargetPosition(-1000);
    break;
  case STATES::MoveTowardsEndStopSlow:
    _stepper->setRPM(SUPER_SLOW_RPM);
    _stepper->startRunContinuous(FORWARD);
    break;
  
  default:
    break;
  }
  _currentState = newState;

  Serial.println("StepperHomingHelper state:");
  Serial.println((char)_currentState);
}

bool StepperHomingHelper::loop()
{
  switch (_currentState)
  {
    case STATES::Start:
    {
      if (digitalRead(_endStopPin) > 0)
        changeState(STATES::EndStopTriggeredAtStart);
      else
        changeState(STATES::MoveTowardsEndStopFast);
    }
    break;
    case STATES::EndStopTriggeredAtStart:
    {
      _stepper->runContinuous();
      if (digitalRead(_endStopPin) <= 0)
        changeState(STATES::MoveTowardsEndStopFast);
    }
    break;
    case STATES::MoveTowardsEndStopFast:
    {
      _stepper->runContinuous();
      if (digitalRead(_endStopPin) > 0)
        changeState(STATES::EndStopTriggeredFirstTime);
    }
    break;
    case STATES::EndStopTriggeredFirstTime:
    {
      _stepper->runContinuous();
      if (digitalRead(_endStopPin) <= 0) 
      {
        changeState(STATES::Backup1000StepsAfterEndStopTriggered);
      }
    }
    break;
    case STATES::Backup1000StepsAfterEndStopTriggered:
    {
      if (!_stepper->runToPosition())
        changeState(STATES::MoveTowardsEndStopSlow);
    }
    break;
    case STATES::MoveTowardsEndStopSlow:
    {
      _stepper->runContinuous();
      if (digitalRead(_endStopPin) > 0)
      {
        _stepper->stop();
        _stepper->resetPosition();
        changeState(STATES::Done);
      }
    }
    break;
  }

  return _currentState == STATES::Done;
}

