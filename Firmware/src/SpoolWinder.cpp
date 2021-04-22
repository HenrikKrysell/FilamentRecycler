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
  changeState(States::MoveToPositionA);
}

void SpoolWinder::loop()
{
  switch (_currentState)
  {
  case States::MoveToPositionA:
  {
    if (!_filamentGuideStepper->runToPosition())
      changeState(MoveToPositionB);
  }
    break;
  case States::MoveToPositionB:
  {
    if (!_filamentGuideStepper->runToPosition())
      changeState(MoveToPositionA);
  }
    break;
  
  default:
    break;
  }
}

void SpoolWinder::changeState(States newState)
{
  switch (newState)
  {
  case States::MoveToPositionA:
    _filamentGuideStepper->setRPM(NORMAL_RPM);
    _filamentGuideStepper->setAbsoluteTargetPosition(-30000);
    _currentState = newState;
  break;
  case States::MoveToPositionB:
    _filamentGuideStepper->setRPM(SLOW_RPM);
    _filamentGuideStepper->setAbsoluteTargetPosition(-10000);
    _currentState = newState;
    break;
  
  default:
    break;
  }
}
