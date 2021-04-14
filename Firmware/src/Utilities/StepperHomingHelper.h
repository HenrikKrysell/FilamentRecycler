#ifndef StepperHomingHelper_h
#define StepperHomingHelper_h

#include <Arduino.h>
#include <AccelStepper.h>
#include "../DataStructures/motorDefinition.h"


class StepperHomingHelper
{
  public:
    StepperHomingHelper(AccelStepper* stepper, MotorDefinition motorDefinition, int endStopPin);
    void start();
    bool loop();

  private:
    enum STATES { 
      Start=0, 
      EndStopTriggeredAtStart, 
      MoveTowardsEndStopFast, 
      EndStopTriggeredFirstTime, 
      MoveTowardsEndStopSlow, 
      Done
    };

    MotorDefinition _motorDefinition;
    int _endStopPin;
    AccelStepper* _stepper;
    STATES _currentState;
};

#endif