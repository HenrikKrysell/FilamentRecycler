#include "Controller.h"


Controller::Controller()
{
  _currentState = ControllerStates::Idle;
  _spoolWinder = new SpoolWinder({.motorDirPin=25, .motorStepPin=23, .motorEnablePin=27}, {.motorDirPin=25, .motorStepPin=23, .motorEnablePin=27}, 10);
}


void Controller::setup()
{
  _spoolWinder->setup();
}

void Controller::changeState(ControllerStates newState)
{
  switch (newState)
  {
  case ControllerStates::Homing:
    {
      _spoolWinder->startHoming();
      _currentState = newState;
    }
    break;
  case ControllerStates::Running:
    {
      if (_currentState == ControllerStates::Idle)
        _currentState = newState;
    }
    break;
  default:
    _currentState = newState;
    break;
  }
}

void Controller::loop()
{
  switch (_currentState)
  {
  case ControllerStates::Homing:
    {
      bool done = _spoolWinder->homingLoop();
      if (done)
        changeState(ControllerStates::Idle);
    }
    break;
  case ControllerStates::Running:
    break;
  case ControllerStates::Idle:
    break;
  default:
    break;
  }
}