#ifndef StepperMotor_h
#define StepperMotor_h

#include <Arduino.h>
#include "../DataStructures/motorDefinition.h"
#include <A4988.h>
#include "../Utilities/Constants.h"


class StepperMotor
{
  public:
    StepperMotor(MotorDefinition motorDefinition);
    ~StepperMotor();

    void start(int defaultRPM = NORMAL_RPM);

    void setRPM(float rpm);
    void resetPosition();

    void startRunContinuous(int direction);
    void runContinuous();

    void setAbsoluteTargetPosition(long absolutePosition);
    void setRelativeTargetPosition(long relativePosition);
    bool runToPosition();

    void stop();
  private:
    MotorDefinition _motorDefinition;
    A4988* _stepper;
    long _currentPosition;
    long _nextActionTime;
};

#endif