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
  _currentState = SpoolWinderStates::Stop;
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

  // DEBUG
  setFilamentGuideStartPos(-9000);
  setFilamentGuideStopPos(-31000);

  setFilamentSpoolDiameter(68);
  setFilamentSpoolWidth(55);

  setSpoolWinderGearRatio(16);
}

void SpoolWinder::startAction(IncommingMessage* msg)
{
  switch (msg->subType)
  {
  case (int)PerformActionSubType::HomeAllAxes:
    changeState(SpoolWinderStates::HomeAllAxes);
    break;
  case (int)PerformActionSubType::MoveAxisAbsolute:
  {
    long filamentGuidePosition = _filamentGuideStepper->getPosition();
    long spoolWinderPosition = _spoolWinderStepper->getPosition();
    getAxesValuesFromMessage(msg, spoolWinderPosition, filamentGuidePosition);
    
    _spoolWinderStepper->setAbsoluteTargetPosition(spoolWinderPosition);
    _filamentGuideStepper->setAbsoluteTargetPosition(filamentGuidePosition);
    changeState(SpoolWinderStates::MoveAxis);
  }
    break;
  case (int)PerformActionSubType::MoveAxisRelative:
  {
    long filamentGuidePosition = 0;
    long spoolWinderPosition = 0;
    getAxesValuesFromMessage(msg, spoolWinderPosition, filamentGuidePosition);
    
    _spoolWinderStepper->setRelativeTargetPosition(spoolWinderPosition);
    _filamentGuideStepper->setRelativeTargetPosition(filamentGuidePosition);
    changeState(SpoolWinderStates::MoveAxis);
  }
    break;
  case (int)PerformActionSubType::Production:
    _lastStepCountGuideMoved = _spoolWinderStepper->getPosition();
    _filamentGuideDirection = -1;
    _numRevolutionsThisLayer = 0;
    _numLayers = 0;
    changeState(SpoolWinderStates::ProductionWaitForSensor);
    break;
  case (int)PerformActionSubType::UnwindSpool:
    changeState(SpoolWinderStates::UnwindSpool);
    break;
  default:
    break;
  }
  

}

LoopStates SpoolWinder::loop()
{
  int potentiometerPosition = _potentiometer->fetchPosition();

  LoopStates result = LoopStates::Working;

  switch (_currentState)
  {
  case SpoolWinderStates::HomeAllAxes:
  {
    if (_filemanetGuideHomingHelper->loop()) {
      result = LoopStates::Done;
    }
  }
  break;
  case SpoolWinderStates::MoveAxis:
  {
    bool spoolWinderDone =_spoolWinderStepper->runToPosition();
    bool filamentGuideDone = _filamentGuideStepper->runToPosition();
    if (spoolWinderDone && filamentGuideDone) {
      result = LoopStates::Done;
    }
  }
  break;
  case SpoolWinderStates::ProductionSpool:
  {

    _spoolWinderStepper->runContinuous();
    // TODO: FilamentGuide
  }
  case SpoolWinderStates::ProductionWaitForSensor:
  {
    _filamentGuideStepper->runToPosition();

    long stepDiff = _spoolWinderStepper->getPosition() - _lastStepCountGuideMoved;
    if (stepDiff >= _spoolWinderNumStepsPerRevolution) {
      _lastStepCountGuideMoved = _spoolWinderStepper->getPosition();

      _numRevolutionsThisLayer++;
      if (_numRevolutionsThisLayer >= _numRevolutionsPerLayer) {
        _numLayers++;
        _numRevolutionsThisLayer = 0;
        // Turn around, but we want to stay for one more revolution to start the next layer at the same spot.
        // Adding one here will make it go from -1 to 0 and then next revolution from 0 to 1 and start moving again.
        _filamentGuideDirection *= -1;
      }
      else {
        _filamentGuideStepper->setRelativeTargetPosition(_filamentGuideDirection * _filamentGuideStepsPerSpoolRevolution);
      }
    }

    if (potentiometerPosition >= POTENTIOMETER_MAX)
      changeState(SpoolWinderStates::ProductionSpool);
    else if (potentiometerPosition >= POTENTIOMETER_MIDDLE)
      changeState(SpoolWinderStates::ProductionSpool);

    if (potentiometerPosition < POTENTIOMETER_MIN)
      changeState(SpoolWinderStates::ProductionWaitForSensor);
  }
  break;
  case SpoolWinderStates::UnwindSpool:
    _spoolWinderStepper->runContinuous();
    break;
  case SpoolWinderStates::Idle:
  case SpoolWinderStates::Stop:
    result = LoopStates::Done;
  default:
    break;
  }

  return result;
}

void SpoolWinder::changeState(SpoolWinderStates newState)
{
  if (newState == _currentState)
    return;

  switch (newState)
  {
    case SpoolWinderStates::HomeAllAxes:
      _filemanetGuideHomingHelper->start();
    break;
  case SpoolWinderStates::MoveAxis:
  break;
  case SpoolWinderStates::Stop:
  case SpoolWinderStates::Idle:
    _filamentGuideStepper->stop();

    _spoolWinderStepper->stop();
  break;
  case SpoolWinderStates::ProductionSpool:
    _filamentGuideStepper->setRPM(FAST_RPM);

    _spoolWinderStepper->setRPM(FAST_RPM);
    _spoolWinderStepper->startRunContinuous(FORWARD);
    break;
  case SpoolWinderStates::ProductionWaitForSensor:
    break;
  case SpoolWinderStates::UnwindSpool:
    _spoolWinderStepper->setRPM(FAST_RPM);
    _spoolWinderStepper->startRunContinuous(BACKWARD);
    break;
  default:
    break;
  }

  _currentState = newState;
}

void SpoolWinder::stop() {
  
}

void SpoolWinder::getAxesValuesFromMessage(IncommingMessage* msg, long &spoolWinderPosition, long &filamentGuidePosition)
{
    for (int i = 0; i < msg->numParams; i++)
    {
      if (msg->parameters[i].name == (char)MoveAxisActionParams::FilamentGuide) {
        filamentGuidePosition = msg->parameters[i].longValue;
      }
      if (msg->parameters[i].name == (char)MoveAxisActionParams::FilamentSpooler) {
        spoolWinderPosition = msg->parameters[i].longValue;
      }
    }
}