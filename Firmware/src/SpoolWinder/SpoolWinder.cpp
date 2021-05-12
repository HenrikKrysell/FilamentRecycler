#include "SpoolWinder.h"

const int A4988InterfaceType = 1;

SpoolWinder::SpoolWinder(MotorDefinition spoolWinderMotorDef, MotorDefinition filamentGuideMotorDef, int filamentGuideEndStopPin, int tensionerPotentiometerPin)
{
  _spoolWinderMotorDef = spoolWinderMotorDef;
  _filamentGuideMotorDef = filamentGuideMotorDef;
  _filamentGuideEndStopPin = filamentGuideEndStopPin;
  _tensionerPotentiometerPin = tensionerPotentiometerPin;

  _spoolWinderStepper = new StepperMotor(_spoolWinderMotorDef);
  _filamentGuideStepper = new StepperMotor(_filamentGuideMotorDef);

  _filemanetGuideHomingHelper = new StepperHomingHelper(_filamentGuideStepper, _filamentGuideMotorDef, _filamentGuideEndStopPin);
  _isWinding = false;

  _potentiometer = new Potentiometer(tensionerPotentiometerPin, 500);
  _intervalTimer.setInterval(5000);
  _currentState = States::Stoped;
}
 
 SpoolWinder::~SpoolWinder()
 {
   delete _spoolWinderStepper;
   delete _filamentGuideStepper;
   delete _filemanetGuideHomingHelper;
 }

void SpoolWinder::setup()
{
  pinMode(_tensionerPotentiometerPin, INPUT);

  _spoolWinderStepper->start();
  _filamentGuideStepper->start();
  _potentiometer->setup();
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
  changeState(States::Idle);
}

void SpoolWinder::loop()
{
  int potentiometerPosition = _potentiometer->fetchPosition();

  // DEBUG
  if (_intervalTimer.isTriggered())
  {
    Serial.print("SpoolWinder potentiometer: ");
    Serial.println(potentiometerPosition);
  }

  switch (_currentState)
  {
  case States::SpoolSlow:
  case States::SpoolFast:
  {
    _spoolWinderStepper->runContinuous();
  }
  case States::Idle:
  {
      if (potentiometerPosition >= POTENTIOMETER_MAX)
        changeState(States::SpoolFast);
      else if (potentiometerPosition >= POTENTIOMETER_MIDDLE)
        changeState(States::SpoolSlow);

      if (potentiometerPosition < POTENTIOMETER_MIN)
        changeState(States::Idle);
  }
    break;
  case States::Stoped:
  default:
    break;
  }
}

void SpoolWinder::changeState(States newState)
{
  if (newState == _currentState)
    return;

  switch (newState)
  {
  case States::Idle:
    _filamentGuideStepper->stop();

    _spoolWinderStepper->stop();

    _currentState = newState;
  break;
  case States::SpoolSlow:
    //_filamentGuideStepper->setAbsoluteTargetPosition(-10000);
    _filamentGuideStepper->setRPM(SUPER_SLOW_RPM);

    _spoolWinderStepper->setRPM(FAST_RPM);
    _spoolWinderStepper->startRunContinuous(FORWARD);

    _currentState = newState;
    break;
  case States::SpoolFast:
    _filamentGuideStepper->setRPM(FAST_RPM);

    _spoolWinderStepper->setRPM(FAST_RPM);
    _spoolWinderStepper->startRunContinuous(FORWARD);

    _currentState = newState;
    break;
  
  default:
    break;
  }

  Serial.print("SpoolWinder state: ");
  Serial.println(SpoolWinderStatesNames[_currentState]);
}
