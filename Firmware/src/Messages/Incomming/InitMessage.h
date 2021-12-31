#ifndef InitMessage_h
#define InitMessage_h

#include "../../MotorRotationDirection.h"

struct InitMessage
{
  byte messageType;

  byte motorINAPin;
  byte motorINBPin;
  byte motorPWMPin;
  byte motorDir; // 0 = clockwise, 1 = Counter clockwise

  byte motorRPMCounterPin;

  byte pullerStepperEnablePin;
  byte pullerStepperStepPin;
  byte pullerStepperDirPin;
  byte pullerStepperDirValue;

  byte winderStepperEnablePin;
  byte winderStepperStepPin;
  byte winderStepperDirPin;
  byte winderStepperDirValue;

  byte filamentGuideEnablePin;
  byte filamentGuideStepPin;
  byte filamentGuideDirPin;
  byte filamentGuideDirValue;

  byte thermistorAnalogPin;
  byte heaterPin;
  byte winderArmAnalogPin;
  byte filamentDiameterAnalogPin;
  byte filamentGuideEndstopPin;

  int telemetricDataIntervalMs;
  int thermistorPollIntervalMs;
  int winderArmPollIntervalMs;
  int filamentDiameterPollIntervalMs;
  int motorRPMCounterPollIntervalMs;
};

#endif