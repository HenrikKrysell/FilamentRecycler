#ifndef StepperMotor_h
#define StepperMotor_h

#include <Arduino.h>
#include "../DataStructures/motorDefinition.h"
#include <A4988.h>
#include "../Utilities/Constants.h"


class StepperMotor : private A4988
{
  public:
    StepperMotor(MotorDefinition motorDefinition);
    ~StepperMotor();

    void start(int defaultRPM = NORMAL_RPM);

    void setRPM(float rpm);
    float getRPM();
    void resetPosition();

    void startRunContinuous(int direction);
    void runContinuous();

    void setAbsoluteTargetPosition(long absolutePosition);
    void setRelativeTargetPosition(long relativePosition);
    bool runToPosition();

    void stop();

    inline long getPosition() __attribute__((always_inline)) {
        return _currentPosition;
    };

  private:
    enum class Mode {
      IDLE = 0,
      CONTINUOUS,
      POSITION,
    };

    MotorDefinition _motorDefinition;
    long _currentPosition;
    long _timeUntilNextAction;
    unsigned long _lastActionTime;
    Mode _mode;
};

#endif