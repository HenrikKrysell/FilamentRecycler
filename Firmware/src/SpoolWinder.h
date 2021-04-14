#ifndef SpoolWinder_h
#define SpoolWinder_h

#include <Arduino.h>
#include <AccelStepper.h>
#include "DataStructures/motorDefinition.h"
#include "Utilities/StepperHomingHelper.h"

class SpoolWinder
{
  public:
    SpoolWinder(MotorDefinition spoolWinderMotorDef, MotorDefinition filamentGuideMotorDef, int filamentGuideEndStopPin);
    void setup();
    void startHoming();
    bool homingLoop();
    void loop();

  private:
    MotorDefinition _spoolWinderMotorDef;
    MotorDefinition _filamentGuideMotorDef;
    int _filamentGuideEndStopPin;
    bool _isWinding;
    bool _isHoming;
    bool _homingDone;
    AccelStepper* _spoolWinderStepper;
    AccelStepper* _filamentGuideStepper;
    StepperHomingHelper* _filemanetGuideHomingHelper;
};

#endif