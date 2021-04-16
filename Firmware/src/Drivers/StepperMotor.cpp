#include "StepperMotor.h"

StepperMotor::StepperMotor(MotorDefinition motorDefinition)
{
  _motorDefinition = motorDefinition;
  _stepper = new A4988(MOTOR_STEPS, _motorDefinition.motorDirPin, _motorDefinition.motorStepPin, _motorDefinition.motorEnablePin);
  _stepper->setEnableActiveState(LOW);
  _nextActionTime = 0;
  _currentPosition = 0;
}

StepperMotor::~StepperMotor()
{
  _stepper->stop();
  delete _stepper;
}

void StepperMotor::start(int defaultRPM)
{
  _stepper->begin(defaultRPM, MICROSTEPS);
  _stepper->enable();
}

void StepperMotor::setRPM(float rpm)
{
  _stepper->setRPM(rpm);

  Serial.println(_stepper->getRPM());
}

void StepperMotor::runContinuous()
{
  long diff = _nextActionTime - micros();
  if (diff < MICROS_DIFF_INTERVAL)
  {
    _nextActionTime = _stepper->nextAction();
    _currentPosition += _stepper->getDirection();
  }
}

void StepperMotor::startRunContinuous(int direction)
{
  if (direction >= 0)
    _stepper->startMove(LONG_MAX);
  else
    _stepper->startMove(-LONG_MAX);
}

void StepperMotor::setAbsoluteTargetPosition(long absolutePosition)
{
  long relativePosition = absolutePosition - _currentPosition;
  _stepper->startMove(relativePosition);
}

void StepperMotor::setRelativeTargetPosition(long relativePosition)
{
  _stepper->startMove(relativePosition);
}


bool StepperMotor::runToPosition()
{
  long diff = _nextActionTime - micros();
  if (diff < MICROS_DIFF_INTERVAL)
  {
    _nextActionTime = _stepper->nextAction();
    _currentPosition += _stepper->getDirection();
  }

  return _nextActionTime > 0;
}

void StepperMotor::stop()
{
  _stepper->stop();
}

void StepperMotor::resetPosition()
{
  _currentPosition = 0;
}