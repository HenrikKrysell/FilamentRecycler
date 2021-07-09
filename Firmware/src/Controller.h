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
#include "CommunicationProtocol/Incomming/IncommingMessage.h"
#include "CommunicationProtocol/Outgoing/OutgoingMessage.h"
#include "CommunicationProtocol/Outgoing/Errors.h"
#include "CommunicationProtocol/Outgoing/Ack.h"
#include "CommunicationProtocol/Incomming/PerformAction.h"
#include "CommunicationProtocol/Incomming/SetParams.h"
#include "CommunicationProtocol/Incomming/GetParams.h"

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
    void handleMessage(IncommingMessage* msg);
    void sendTelemetryData();
    void sendParameterData(char parameterId);
    void sendParams(IncommingMessage *msg);
    void setParams(IncommingMessage *msg);
    void clearCurrentAction();


    ControllerStates _currentState;
    MotorDefinition _pullerMotorDef;
    int _filamentGuideEndStopPin;
    SpoolWinder* _spoolWinder;
    SerialCommandParser _serialCommandParser;
    FilamentExtruder* _filamentExtruder;
    IntervalTimer _sendTelemetryTimer;
    char _telemetryDataVaribles[20];
    IncommingMessage* _currentAction;
    // DEBUG
    IntervalTimer _intervalTimer;

};

#endif