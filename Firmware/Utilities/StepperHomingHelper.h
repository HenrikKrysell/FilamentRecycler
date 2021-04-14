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
    void stop();

  private:
    enum STATES { 
      START=0, 
      EndStopTriggeredAtStart, 
      MoveTowardsEndStopFast, 
      EndStopTriggeredFirstTime, 
      BackUpFromEndStop, 
      MoveTowardsEndStopSlow, 
      EndStopTriggeredSecondTime 
    };

    MotorDefinition _motorDefinition;
    int _endStopPin;
    bool _isHoming;
    bool _homingDone;
    AccelStepper* _stepper;
    STATES _currentState;
};

#endif