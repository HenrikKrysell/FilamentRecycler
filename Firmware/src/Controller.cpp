#include "Controller.h"


Controller::Controller()
{
  _currentState = ControllerStates::Idle;
  _spoolWinder = new SpoolWinder({.motorDirPin=33, .motorStepPin=31, .motorEnablePin=29}, {.motorDirPin=23, .motorStepPin=25, .motorEnablePin=27}, 24, A10);
  _pullerMotorDef = {.motorDirPin=47, .motorStepPin=49, .motorEnablePin=48};
  _pullerMotorStepper = new StepperMotor(_pullerMotorDef);
}

Controller::~Controller()
{
  delete _spoolWinder;
  delete _pullerMotorStepper;
}

void Controller::setup()
{
  _spoolWinder->setup();
  _pullerMotorStepper->start();

  // DEBUG
  _intervalTimer.setInterval(10000);

  Serial.println("Controller Setup Complete");
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

        _pullerRPM = 1;
        _pullerMotorStepper->setRPM(_pullerRPM);
        _pullerMotorStepper->startRunContinuous(BACKWARD);
      }
      else 
      {
        Serial.println("ERROR: Cannot change state to running unless we are in Idle state");
      }
    }
    break;
  case ControllerStates::Idle:
    _currentState = newState;
  break;
  default:  
    Serial.print("ERROR: Invalid controller state: ");
    Serial.println(newState);
    _currentState = ControllerStates::Idle;
    break;
  }

  Serial.print("Controller state: ");
  Serial.println(ControllerStatesNames[_currentState]);
}

void Controller::loop()
{
  Command cmd = _serialCommandParser.readIfDataPresent();
  if (cmd.type != CommandType::Nothing)
    PerformCommand(cmd);

  StateMachineLoop();
}

void Controller::PerformCommand(Command cmd)
{
  switch (cmd.type)
  {
  case CommandType::ChangeState:
  {
    if (cmd.parameters[0].name != 'S')
      Serial.println("ERROR: Change state command only takes one parameter named S");
    else
      changeState((ControllerStates)cmd.parameters[0].intValue);
  }
  break;
  case CommandType::SetSpeed:
  {

  }
  break;
  case  CommandType::SetTemperature:
  {

  }
  break;
  case CommandType::Nothing:
  default:
    break;
  }

  // Serial.println("== Command ==");
  // Serial.println(cmd.type);
  // for (int i = 0; i < cmd.numParams; i++)
  // {
  //   Serial.println("-Param");
  //   Serial.println(cmd.parameters[i].name);
  //   Serial.println(cmd.parameters[i].floatValue);
  //   Serial.println(cmd.parameters[i].intValue);
  // }
  // Serial.println("===========");
}

void Controller::StateMachineLoop()
{
  switch (_currentState)
  {
  case ControllerStates::Homing:
    {
      bool done = _spoolWinder->homingLoop();
      if (done)
      {
        changeState(ControllerStates::Idle);
      }
    }
    break;
  case ControllerStates::Running:
  {
    if (_intervalTimer.isTriggered())
    {
      char buffer[30];
      _pullerMotorStepper->setRPM(_pullerRPM);
      dtostrf(_pullerMotorStepper->getRPM(), 10, 6, buffer);
      Serial.print("RPM: ");
      Serial.println(buffer);
      _pullerRPM += 10.0f;
    }

    _spoolWinder->loop();
    _pullerMotorStepper->runContinuous();

  }
    break;
  case ControllerStates::Idle:
    break;
  default:
    break;
  }
}