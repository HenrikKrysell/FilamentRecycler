#include "Controller.h"


Controller::Controller()
{
  _currentState = ControllerStates::Idle;
  _spoolWinder = new SpoolWinder({.motorDirPin=33, .motorStepPin=31, .motorEnablePin=29}, {.motorDirPin=23, .motorStepPin=25, .motorEnablePin=27}, 24);
}

Controller::~Controller()
{
  delete _spoolWinder;
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
      {
        _currentState = newState;
        _spoolWinder->startLoop();
      }
    }
    break;
  default:
    _currentState = newState;
    break;
  }
  Serial.println("Controller state:");
  Serial.println((char)_currentState);
}

void Controller::loop()
{
  switch (_currentState)
  {
  case ControllerStates::Homing:
    {
      bool done = _spoolWinder->homingLoop();
      if (done)
      {
        changeState(ControllerStates::Idle);
        changeState(ControllerStates::Running);
      }
    }
    break;
  case ControllerStates::Running:
  {
    if (!_spoolWinder->loop())
    {
      Serial.println("Reached position... waiting");
      delay(2000);
      changeState(ControllerStates::Homing);
    }
  }
    break;
  case ControllerStates::Idle:
    break;
  default:
    break;
  }
}