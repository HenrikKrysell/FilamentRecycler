#ifndef StepperHomingHelper_h
#define StepperHomingHelper_h

#include <Arduino.h>
#include "../DataStructures/motorDefinition.h"
#include <A4988.h>
#include "Constants.h"
#include "../Drivers/StepperMotor.h"


class StepperHomingHelper
{
  public:
    StepperHomingHelper(StepperMotor* stepper, MotorDefinition motorDefinition, int endStopPin);
    void start();
    bool loop();

  private:
    enum STATES { 
      Start = 'S', 
      EndStopTriggeredAtStart = 'X', 
      MoveTowardsEndStopFast = 'F', 
      EndStopTriggeredFirstTime = 'E',
      Backup1000StepsAfterEndStopTriggered = 'B',
      MoveTowardsEndStopSlow = 'M', 
      Done = 'D'
    };

    void changeState(STATES newState);
    
    MotorDefinition _motorDefinition;
    int _endStopPin;
    StepperMotor* _stepper;
    STATES _currentState;
};

#endif