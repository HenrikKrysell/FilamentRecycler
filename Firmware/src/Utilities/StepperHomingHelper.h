#ifndef StepperHomingHelper_h
#define StepperHomingHelper_h

#include <Arduino.h>
#include "../DataStructures/motorDefinition.h"
#include <A4988.h>
#include "Constants.h"
#include "../Drivers/StepperMotor.h"

const static char* StatesNames[] = {
  "Start", 
  "EndStopTriggeredAtStart", 
  "MoveTowardsEndStopFast", 
  "EndStopTriggeredFirstTime", 
  "Backup1000StepsAfterEndStopTriggered", 
  "MoveTowardsEndStopSlow", 
  "Done"
};


class StepperHomingHelper
{
  public:
    StepperHomingHelper(StepperMotor* stepper, MotorDefinition motorDefinition, int endStopPin);
    void start();
    bool loop();

  private:
    enum STATES { 
      Start = 0, 
      EndStopTriggeredAtStart,
      MoveTowardsEndStopFast,
      EndStopTriggeredFirstTime,
      Backup1000StepsAfterEndStopTriggered,
      MoveTowardsEndStopSlow,
      Done
    };

    void changeState(STATES newState);
    
    MotorDefinition _motorDefinition;
    int _endStopPin;
    StepperMotor* _stepper;
    STATES _currentState;
};

#endif