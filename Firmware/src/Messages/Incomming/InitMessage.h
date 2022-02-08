#ifndef InitMessage_h
#define InitMessage_h

#include "../../MotorRotationDirection.h"

struct StepperConfig
{
  byte enablePin;
  byte stepPin;
  byte dirPin;
  byte dirValue;
} __attribute__((__packed__));

struct OlimexMotorDriverConfig
{
  byte inAPin;
  byte inBPin;
  byte pwmPin;
  byte dir; // 0 = clockwise, 1 = Counter clockwise
} __attribute__((__packed__));

struct AnalogInputConfig
{
  int pollIntervalMs;
  int delayBetweenFirstPoll;
  int delayBetweenSamples;
  byte pin;
  byte numSamples;
} __attribute__((__packed__));

struct CounterConfig
{
  byte pin;
  int pollIntervalMs;
  int delayBetweenFirstPoll;
} __attribute__((__packed__));

struct InitMessage
{
  byte messageType;

  byte heaterPin;
  byte filamentGuideEndstopPin;
  int telemetricDataIntervalMs;

  OlimexMotorDriverConfig motorDriver;
  CounterConfig motorRPMCounter;

  StepperConfig pullerStepper;
  StepperConfig winderStepper;
  StepperConfig filamentGuideStepper;

  AnalogInputConfig thermistorConfig;
  AnalogInputConfig winderArmConfig;
  AnalogInputConfig filamentDiamterConfig;
} __attribute__((__packed__));

#endif