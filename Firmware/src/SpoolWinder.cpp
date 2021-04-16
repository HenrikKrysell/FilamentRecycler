#include "SpoolWinder.h"

const int A4988InterfaceType = 1;

SpoolWinder::SpoolWinder(MotorDefinition spoolWinderMotorDef, MotorDefinition filamentGuideMotorDef, int filamentGuideEndStopPin)
{
  _spoolWinderMotorDef = spoolWinderMotorDef;
  _filamentGuideMotorDef = filamentGuideMotorDef;
  _filamentGuideEndStopPin = filamentGuideEndStopPin;

  _spoolWinderStepper = new StepperMotor(_spoolWinderMotorDef);
  _filamentGuideStepper = new StepperMotor(_filamentGuideMotorDef);

  _filemanetGuideHomingHelper = new StepperHomingHelper(_filamentGuideStepper, _filamentGuideMotorDef, _filamentGuideEndStopPin);
  _isWinding = false;
}
 
 SpoolWinder::~SpoolWinder()
 {
   delete _spoolWinderStepper;
   delete _filamentGuideStepper;
   delete _filemanetGuideHomingHelper;
 }

void SpoolWinder::setup()
{
  _spoolWinderStepper->start();
  _filamentGuideStepper->start();
}

void SpoolWinder::startHoming()
{
  _filemanetGuideHomingHelper->start();
}

bool SpoolWinder::homingLoop()
{
  return _filemanetGuideHomingHelper->loop();
}

void SpoolWinder::startLoop()
{
  _filamentGuideStepper->setRPM(NORMAL_RPM);
  _filamentGuideStepper->setAbsoluteTargetPosition(-20000);
}

bool SpoolWinder::loop()
{
  return _filamentGuideStepper->runToPosition();
  // if (_isWinding)
  // {
  //   int maxLimitValue = digitalRead(_maxLimitSwitchPin);
  //   if (maxLimitValue > 0)
  //     _isWinding = false;
  // }
  // else
  // {
  //   int minLimitValue = digitalRead(_minLimitSwitchPin);
  //   if (minLimitValue > 0)
  //     _isWinding = true;
  // }
    
  // if (_isWinding)
  //   _stepper->runSpeed();
}