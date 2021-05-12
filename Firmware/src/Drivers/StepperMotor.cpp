#include "StepperMotor.h"

StepperMotor::StepperMotor(MotorDefinition motorDefinition)
  : A4988(MOTOR_STEPS, motorDefinition.motorDirPin, motorDefinition.motorStepPin, motorDefinition.motorEnablePin)
{
  _mode = Mode::IDLE;
  _motorDefinition = motorDefinition;
  setEnableActiveState(LOW);
  _timeUntilNextAction = 0;
  _currentPosition = 0;
  _lastActionTime = 0;
}

StepperMotor::~StepperMotor()
{
  stop();
}

void StepperMotor::start(int defaultRPM)
{
  begin(defaultRPM, MICROSTEPS);
  enable();
}

void StepperMotor::setRPM(float rpm)
{
  switch (_mode)
  {
  case Mode::IDLE:
    A4988::setRPM(rpm);
    break;
  case Mode::CONTINUOUS:
  {
    A4988::setRPM(rpm);
    startRunContinuous(getDirection());
  }
    break;
  case Mode::POSITION:
  {
    A4988::setRPM(rpm);
    setRelativeTargetPosition(getDirection() * steps_remaining);
  }
    break;
  default:
    break;
  }
}

float StepperMotor::getRPM()
{
  return A4988::getRPM();
}

void StepperMotor::runContinuous()
{
  if ((micros() - _lastActionTime) >= _timeUntilNextAction)
  {
    _timeUntilNextAction = nextAction();
    _lastActionTime = micros();
    _currentPosition += getDirection();
    steps_remaining = 10;
  }
}

void StepperMotor::startRunContinuous(int direction)
{
  _mode = Mode::CONTINUOUS;
  if (direction >= 0)
    startMove(10);
  else
    startMove(-10);
}

void StepperMotor::setAbsoluteTargetPosition(long absolutePosition)
{
  _mode = Mode::POSITION;
  long relativePosition = absolutePosition - _currentPosition;
  startMove(relativePosition);
}

void StepperMotor::setRelativeTargetPosition(long relativePosition)
{
  _mode = Mode::POSITION;
  startMove(relativePosition);
}

bool StepperMotor::runToPosition()
{
  if ((micros() - _lastActionTime) >= _timeUntilNextAction)
  {
    _timeUntilNextAction = nextAction();
    _lastActionTime = micros();
    _currentPosition += getDirection();
  }

  if (_timeUntilNextAction <= 0) {
    _mode = Mode::IDLE;
  }

  return _timeUntilNextAction > 0;
}

void StepperMotor::stop()
{
  _mode = Mode::IDLE;
  A4988::stop();
}

void StepperMotor::resetPosition()
{
  _currentPosition = 0;
}