#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
#include "DataStructures/motorDefinition.h"
#include "SpoolWinder.h"

enum ControllerStates {
  Idle = 'I',
  Homing = 'H',
  Running = 'R',
};

class Controller
{
  public:
    Controller();
    ~Controller();

    void setup();
    void loop();

    void changeState(ControllerStates newState);

  private:
    ControllerStates _currentState;
    MotorDefinition _spoolWinderMotorDef;
    MotorDefinition _filamentGuideMotorDef;
    int _filamentGuideEndStopPin;
    SpoolWinder* _spoolWinder;
};

#endif