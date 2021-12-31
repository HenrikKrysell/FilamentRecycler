#ifndef SetValuesMessage_h
#define SetValuesMessage_h

struct SetValuesMessage
{
  // turn on heater?
  byte messageType;
  byte augerMotorSpeed;
  byte winderStepperRPM;
  byte pullerStepperRPM;
  char filamentGuideSteps;
};

#endif