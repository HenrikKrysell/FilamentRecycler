#ifndef SetValuesMessage_h
#define SetValuesMessage_h

struct SetValuesMessageHeader
{
  byte messageType: 4;
  bool turnOnHeater: 1;
  bool emergencyStop: 1;
} __attribute__((__packed__));

struct SetValuesMessage
{
  SetValuesMessageHeader messageHeader;
  byte augerMotorSpeed;
  byte winderStepperRPM;
  byte pullerStepperRPM;
  char filamentGuideSteps;
} __attribute__((__packed__));

#endif