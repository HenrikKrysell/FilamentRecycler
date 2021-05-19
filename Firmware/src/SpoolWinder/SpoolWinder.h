#ifndef SpoolWinder_h
#define SpoolWinder_h

#include <Arduino.h>
#include "../DataStructures/motorDefinition.h"
#include "../Utilities/StepperHomingHelper.h"
#include <A4988.h>
#include "../Utilities/Constants.h"
#include "../Drivers/StepperMotor.h"
#include "Potentiometer.h"
#include "../Utilities/IntervalTimer.h"
#include "../Utilities/LoopState.h"
#include "../CommunicationProtocol/Protocol.h"
#include "../CommunicationProtocol/PerformAction.h"

enum class SpoolWinderStates {
  Idle = 0,
  Stop,
  ProductionSpool,
  ProductionWaitForSensor,
  MoveAxis,
  HomeAllAxes,
};
const static char* SpoolWinderStatesNames[] = { "0:Idle", "1:Stopped", "2:SpoolSlow", "3:SpoolFast" };

class SpoolWinder
{
  public:
    SpoolWinder(MotorDefinition spoolWinderMotorDef, MotorDefinition filamentGuideMotorDef, int filamentGuideEndStopPin, int tensionerPotentiometerPin);
    ~SpoolWinder();
    
    void setup();
    void stop();

    void startAction(Message* msg);
    LoopStates loop();

    inline SpoolWinderStates getState() __attribute__((always_inline)) {
        return _currentState;
    };

    inline long getFilamentGuidePosition() __attribute__((always_inline)) {
        return _filamentGuideStepper->getPosition();
    };

  private:

    void changeState(SpoolWinderStates newState);
    void getAxesValuesFromMessage(Message* msg, long &spoolWinderPosition, long &filamentGuidePosition);

    MotorDefinition _spoolWinderMotorDef;
    MotorDefinition _filamentGuideMotorDef;
    int _filamentGuideEndStopPin;
    int _tensionerPotentiometerPin;
    bool _isWinding;
    bool _isHoming;
    bool _homingDone;
    StepperMotor* _spoolWinderStepper;
    StepperMotor* _filamentGuideStepper;
    StepperHomingHelper* _filemanetGuideHomingHelper;
    SpoolWinderStates _currentState;
    Potentiometer* _potentiometer;
    // DEBUG
    IntervalTimer _intervalTimer;
    float dummyFloat = 1234.567;
};

#endif