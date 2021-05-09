#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
#include "DataStructures/motorDefinition.h"
#include "SpoolWinder/SpoolWinder.h"
#include "SerialCommandParser.h"
#include "DataStructures/Command.h"
#include "Drivers/StepperMotor.h"
#include "Utilities/IntervalTimer.h"
#include <A4988.h>

enum ControllerStates {
  Idle = 0,
  Homing = 1,
  Running = 2,
};
const static char* ControllerStatesNames[] = { "0:Idle", "1:Homing", "2:Running" };

class Controller
{
  public:
    Controller();
    ~Controller();

    void setup();
    void loop();

    void changeState(ControllerStates newState);

  private:
    void PerformCommand(Command cmd);
    void StateMachineLoop();


    ControllerStates _currentState;
    MotorDefinition _pullerMotorDef;
    int _filamentGuideEndStopPin;
    SpoolWinder* _spoolWinder;
    SerialCommandParser _serialCommandParser;
    StepperMotor* _pullerMotorStepper;
    float _pullerRPM;
    // DEBUG
    IntervalTimer _intervalTimer;

};

#endif