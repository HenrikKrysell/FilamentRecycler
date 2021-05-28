#include "Controller.h"


Controller::Controller()
{
  _currentState = ControllerStates::Idle;
  _spoolWinder = new SpoolWinder({.motorDirPin=33, .motorStepPin=31, .motorEnablePin=29}, {.motorDirPin=23, .motorStepPin=25, .motorEnablePin=27}, 24, A10);
  _filamentExtruder = new FilamentExtruder({.motorDirPin=47, .motorStepPin=49, .motorEnablePin=48});
  _currentAction = NULL;
}

Controller::~Controller()
{
  delete _spoolWinder;
  delete _filamentExtruder;
}

void Controller::setup()
{
  _spoolWinder->setup();
  _filamentExtruder->setup();

  _sendTelemetryTimer.setInterval(9999);
  _telemetryDataVaribles[0] = '\0';

  // DEBUG
  _intervalTimer.setInterval(10000);


  // Serial.println("Controller Setup Complete");
  // Serial.println("Avilable commands:");
  // for (int i = 0; i < CommandType::CommandTypeEnd; i++)
  // {
  //   Serial.println(CommandExplanation[i]);
  // }
  
}

void Controller::changeState(ControllerStates newState)
{
  switch (newState)
  {
  case ControllerStates::Stop:
    {
      _spoolWinder->stop();
      _filamentExtruder->stop();
      _currentState = newState;
      if (_currentAction != NULL) {
        sendActionResult(_currentAction->id, ResultActionParams::Stopped);
        clearCurrentAction();
      }
    }
    break;
  case ControllerStates::PerformAction:
    {
      if (_currentState == ControllerStates::Idle || _currentState == ControllerStates::Stop)
      {
        if (_currentAction == NULL) {
          SerialSendError(ERROR_CONTROLLER_NO_ACTION_TO_PERFORM);
          _currentState = ControllerStates::Stop;
          break;
        }
        _currentState = newState;
        
        _spoolWinder->startAction(_currentAction);
        _filamentExtruder->startAction(_currentAction);
      }
      else 
      {
        SerialSendError(ERROR_CONTROLLER_CANNOT_CHANGE_STATE);
      }
    }
    break;
  case ControllerStates::Idle:
    _currentState = newState;
  break;
  default:  
    SerialSendError(ERROR_CONTROLLER_INVALID_STATE, (int)newState);
    _currentState = ControllerStates::Idle;
    break;
  }
}

void Controller::loop()
{
  Message* msg = _serialCommandParser.readIfDataPresent();
  if (msg != NULL)
    handleMessage(msg);

  switch (_currentState)
  {
  case ControllerStates::PerformAction:
  {
    if (_intervalTimer.isTriggered()) {
      // char buffer[30];
      // _pullerMotorStepper->setRPM(_pullerRPM);
      // dtostrf(_pullerMotorStepper->getRPM(), 10, 6, buffer);
      // Serial.print("RPM: ");
      // Serial.println(buffer);
      // _pullerRPM += 10.0f;
    }

    LoopStates spoolWinderState =_spoolWinder->loop();
    LoopStates filamentExtruderState = _filamentExtruder->loop();
    if (spoolWinderState == LoopStates::Done && filamentExtruderState == LoopStates::Done) {
      if (_currentAction != NULL) {
        sendActionResult(_currentAction->id, ResultActionParams::Success);
        clearCurrentAction();
      }
      changeState(ControllerStates::Idle);
    }
    else if (spoolWinderState == LoopStates::Error || filamentExtruderState == LoopStates::Error) {
      if (_currentAction != NULL) {
        sendActionResult(_currentAction->id, ResultActionParams::Error);
        clearCurrentAction();
      }
      changeState(ControllerStates::Stop);
    }
  }
  break;
  case ControllerStates::Stop:
  case ControllerStates::Idle:
    _spoolWinder->stop();
    _filamentExtruder->stop();
    break;
  default:
    break;
  }

  sendTelemetryData();
}

void Controller::handleMessage(Message* msg)
{
  switch (msg->type)
  {
  case MessageType::Telemetry:
  {
    for (int i = 0; i < msg->numParams; i++) 
    {
      _telemetryDataVaribles[i] = msg->parameters[i].name;
    }
    _telemetryDataVaribles[msg->numParams] = '\0';

  }
  break;
  case MessageType::PerformAction:
  {
    if (_currentState == ControllerStates::PerformAction) {
      SerialSendError(ERROR_CONTROLLER_OPERATION_CANNOT_BE_PERFORMED_IN_CURRENT_STATE);
      return;
    }    

    _currentAction = msg;
    msg = NULL;
    _spoolWinder->startAction(_currentAction);
    _filamentExtruder->startAction(_currentAction);

    changeState(ControllerStates::PerformAction);
  }
  break;
  case  MessageType::SetParams:
    setParams(msg);
  break;
  case MessageType::GetParams:
    sendParams(msg);
  break;
  case MessageType::Stop:
    changeState(ControllerStates::Stop);
  break;
  default:
    break;
  }

  if (msg != NULL) {
    delete msg;
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

void Controller::sendTelemetryData() {
  if (_sendTelemetryTimer.isTriggered()) {

    if (_telemetryDataVaribles[0] == '\0')
      return;

    int index = 0;

    Serial.print((char)MessageType::Telemetry);
    while (_telemetryDataVaribles[index] != '\0')
    {
      Serial.print(" ");
      Serial.print(_telemetryDataVaribles[index]);
      sendParameterData(_telemetryDataVaribles[index]);

      index++;
    }
    Serial.println();
  }
}

void Controller::sendParameterData(char parameterId)
{
    switch (parameterId)
    {
    case (char)GetParamsParams::ControllerState:
      Serial.print((int)_currentState);
      break;
    case (char)GetParamsParams::ExtruderRPM:
      Serial.print(-1);
      break;
    case (char)GetParamsParams::FilamentExtruderState:
      Serial.print((int)_filamentExtruder->getFilamentExtruderState());
      break;
    case (char)GetParamsParams::FilamentGuidePos:
      Serial.print(_spoolWinder->getFilamentGuidePosition());
      break;
    case (char)GetParamsParams::PullerRPM:
      Serial.print(_filamentExtruder->getPullerRPM());
      break;
    case (char)GetParamsParams::SpoolWinderState:
      Serial.print(-1);
      break;
    case (char)GetParamsParams::Temperature:
      Serial.print(-1);
      break;
    case (char)GetParamsParams::FilamentGuideStepsPerSpoolRevolution:
      Serial.print(_spoolWinder->getFilamentGuideStepsPerSpoolRevolution());
      break;
    case (char)GetParamsParams::SpoolWinderNumStepsPerRevolution:
      Serial.print(_spoolWinder->getSpoolWinderNumStepsPerRevolution());
      break;
    case (char)GetParamsParams::ErrorCodes:
    {
    }
    break;
    default:
      break;
    }
}

void Controller::sendParams(Message *msg)
{
  Serial.print((char)msg->type);
  for (int i = 0; i < msg->numParams; i++) 
  {
    Serial.print(" ");
    Serial.print(msg->parameters[i].name);
    sendParameterData(msg->parameters[i].name);
  }
  Serial.println();
}

void Controller::setParams(Message *msg)
{
  for (int i = 0; i < msg->numParams; i++)
  {
    Parameter param = msg->parameters[i];
    switch (param.name)
    {
    case (char)SetParamsParams::Temperature:
      break;
    case (char)SetParamsParams::TelemetryInterval:
      _sendTelemetryTimer.setInterval(param.longValue);
      break;
    case (char)SetParamsParams::PullerRPM:
      _filamentExtruder->setPullerRPM(param.floatValue);
      break;
    case (char)SetParamsParams::FilamentSpoolWidth:
      _spoolWinder->setFilamentSpoolWidth(param.longValue);
      break;
    case (char)SetParamsParams::FilamentSpoolDiameter:
      _spoolWinder->setFilamentSpoolDiameter(param.longValue);
      break;
    case (char)SetParamsParams::FilamentGuideStopPos:
      _spoolWinder->setFilamentGuideStopPos(param.longValue);
      break;
    case (char)SetParamsParams::FilamentGuideStartPos:
      _spoolWinder->setFilamentGuideStartPos(param.longValue);
      break;
    case (char)SetParamsParams::FilamentGuideArmTopPosition:
      break;
    case (char)SetParamsParams::FilamentGuideArmBottomPosition:
      break;
    case (char)SetParamsParams::SpoolWinderGearRatio:
      _spoolWinder->setSpoolWinderGearRatio(param.longValue);
    default:
      break;
    }
  }
  
}

void Controller::clearCurrentAction()
{
  if (_currentAction != NULL) {
    delete _currentAction;
    _currentAction = NULL;
  }
}