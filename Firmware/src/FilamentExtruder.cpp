#include "FilamentExtruder.h"


FilamentExtruder::FilamentExtruder(MotorDefinition pullerMotorDef)
{
  _currentState = FilamentExtruderStates::Idle;
  _pullerMotorDef = pullerMotorDef;
  _pullerMotorStepper = new StepperMotor(_pullerMotorDef);
  _pullerRPM = 40;
}

FilamentExtruder::~FilamentExtruder()
{
  delete _pullerMotorStepper;
}

void FilamentExtruder::setup()
{
  _pullerMotorStepper->start();
}

LoopStates FilamentExtruder::loop()
{
  switch (_currentState)
  {
  case FilamentExtruderStates::Running:
    _pullerMotorStepper->runContinuous();
    return LoopStates::Working;
    break;
  default:
    break;
  }

  return LoopStates::Done;
}

void FilamentExtruder::startAction(IncommingMessage* msg)
{
  switch (msg->subType)
  {
  case (int)PerformActionSubType::Production:
    changeState(FilamentExtruderStates::Running);
    break;
  
  default:
    break;
  }
}

void FilamentExtruder::changeState(FilamentExtruderStates newState)
{
  switch (newState)
  {
  case FilamentExtruderStates::Idle:
    {
      _currentState = newState;
    }
    break;
  case FilamentExtruderStates::Heating:
    {
      if (_currentState == FilamentExtruderStates::Idle || _currentState == FilamentExtruderStates::Stop)
      {
        _currentState = newState;
      }
      else 
      {
        Serial.println("ERROR: Cannot change state to running unless we are in Idle state");
      }
    }
    break;
  case FilamentExtruderStates::Stop:
    _currentState = newState;
  break;
  case FilamentExtruderStates::Running:
    _currentState = newState;
    _pullerMotorStepper->setRPM(_pullerRPM);
    _pullerMotorStepper->startRunContinuous(BACKWARD);

  break;
  default:  
    Serial.print("ERROR: Invalid filament extruder state: ");
    Serial.println((int)newState);
    _currentState = FilamentExtruderStates::Stop;
    break;
  }
}

void FilamentExtruder::stop() {
 changeState(FilamentExtruderStates::Stop);
}