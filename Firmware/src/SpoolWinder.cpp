#include "SpoolWinder.h"

const int A4988InterfaceType = 1;

SpoolWinder::SpoolWinder(MotorDefinition spoolWinderMotorDef, MotorDefinition filamentGuideMotorDef, int filamentGuideEndStopPin)
{
  _spoolWinderMotorDef = spoolWinderMotorDef;
  _filamentGuideMotorDef = filamentGuideMotorDef;
  _filamentGuideEndStopPin = filamentGuideEndStopPin;

  _spoolWinderStepper = new AccelStepper(A4988InterfaceType, _spoolWinderMotorDef.motorStepPin, _spoolWinderMotorDef.motorDirPin);
  _filamentGuideStepper = new AccelStepper(A4988InterfaceType, _filamentGuideMotorDef.motorStepPin, _filamentGuideMotorDef.motorDirPin);
  _filemanetGuideHomingHelper = new StepperHomingHelper(_filamentGuideStepper, _filamentGuideMotorDef, _filamentGuideEndStopPin);
  _isWinding = false;
}

void SpoolWinder::startHoming()
{
  _filemanetGuideHomingHelper->start();
}

 bool SpoolWinder::homingLoop()
{
  return _filemanetGuideHomingHelper->loop();
}

void SpoolWinder::setup()
{
  pinMode(_filamentGuideEndStopPin, INPUT);

  pinMode(_spoolWinderMotorDef.motorEnablePin, OUTPUT);
  digitalWrite(_spoolWinderMotorDef.motorEnablePin, LOW);

  pinMode(_filamentGuideMotorDef.motorEnablePin, OUTPUT);
  digitalWrite(_filamentGuideMotorDef.motorEnablePin, LOW);

  // Set the maximum speed in steps per second:
  _spoolWinderStepper->setMaxSpeed(1000);
  _spoolWinderStepper->setSpeed(900);
  _filamentGuideStepper->setMaxSpeed(1000);
  _filamentGuideStepper->setSpeed(900);
}

void SpoolWinder::loop()
{
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