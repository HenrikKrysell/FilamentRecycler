#ifndef SpoolWinder_h
#define SpoolWinder_h

#include <Arduino.h>
#include "DataStructures/motorDefinition.h"
#include "Utilities/StepperHomingHelper.h"
#include <A4988.h>
#include "Utilities/Constants.h"
#include "Drivers/StepperMotor.h"

class SpoolWinder
{
  public:
    SpoolWinder(MotorDefinition spoolWinderMotorDef, MotorDefinition filamentGuideMotorDef, int filamentGuideEndStopPin);
    ~SpoolWinder();
    
    void setup();
    void startHoming();
    bool homingLoop();

    void startLoop();
    bool loop();

  private:
    MotorDefinition _spoolWinderMotorDef;
    MotorDefinition _filamentGuideMotorDef;
    int _filamentGuideEndStopPin;
    bool _isWinding;
    bool _isHoming;
    bool _homingDone;
    StepperMotor* _spoolWinderStepper;
    StepperMotor* _filamentGuideStepper;
    StepperHomingHelper* _filemanetGuideHomingHelper;
};

#endif