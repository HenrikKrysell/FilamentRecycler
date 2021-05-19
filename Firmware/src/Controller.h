#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
#include "DataStructures/motorDefinition.h"
#include "SpoolWinder/SpoolWinder.h"
#include "Drivers/StepperMotor.h"
#include "Utilities/IntervalTimer.h"
#include <A4988.h>
#include "FilamentExtruder.h"
#include "CommunicationProtocol/SerialCommandParser.h"
#include "CommunicationProtocol/Protocol.h"
#include "CommunicationProtocol/Errors.h"
#include "CommunicationProtocol/PerformAction.h"
#include "CommunicationProtocol/SetParams.h"
#include "CommunicationProtocol/GetParams.h"

enum class ControllerStates {
  Idle = 0,
  Stop = 1,
  PerformAction = 2,
};
const static char* ControllerStatesNames[] = { "0:Idle", "1:Stop", "2:PerformCommad" };

class Controller
{
  public:
    Controller();
    ~Controller();

    void setup();
    void loop();

    void changeState(ControllerStates newState);

  private:
    void handleMessage(Message* msg);
    void sendTelemetryData();
    void sendParameterData(char parameterId);
    void sendParams(Message *msg);
    void setParams(Message *msg);
    void clearCurrentAction();


    ControllerStates _currentState;
    MotorDefinition _pullerMotorDef;
    int _filamentGuideEndStopPin;
    SpoolWinder* _spoolWinder;
    SerialCommandParser _serialCommandParser;
    FilamentExtruder* _filamentExtruder;
    IntervalTimer _sendTelemetryTimer;
    char _telemetryDataVaribles[20];
    Message* _currentAction;
    // DEBUG
    IntervalTimer _intervalTimer;

};

#endif