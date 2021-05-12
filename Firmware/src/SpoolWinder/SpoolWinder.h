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

const static char* SpoolWinderStatesNames[] = { "0:Stopped", "1:Idle", "2:SpoolSlow", "3:SpoolFast" };

class SpoolWinder
{
  public:
    SpoolWinder(MotorDefinition spoolWinderMotorDef, MotorDefinition filamentGuideMotorDef, int filamentGuideEndStopPin, int tensionerPotentiometerPin);
    ~SpoolWinder();
    
    void setup();
    void startHoming();
    bool homingLoop();

    void startLoop();
    void loop();

  private:
    enum States {
      Stoped = 0,
      Idle,
      SpoolSlow,
      SpoolFast,
    };

    void changeState(States newState);

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
    States _currentState;
    Potentiometer* _potentiometer;
    // DEBUG
    IntervalTimer _intervalTimer;
};

#endif