#ifndef FilamentExtruder_h
#define FilamentExtruder_h

#include <Arduino.h>
#include "DataStructures/motorDefinition.h"
#include "CommunicationProtocol/Protocol.h"
#include "CommunicationProtocol/PerformAction.h"
#include "Drivers/StepperMotor.h"
#include "Utilities/IntervalTimer.h"
#include "Utilities/LoopState.h"
#include <A4988.h>


static const char* FilamentExtruderStatesNames[] = {"0:Idle", "1:Stop", "2:Heating", "3:Running"};
enum class FilamentExtruderStates {
    Idle = 0,
    Stop,
    Heating,
    Running,
};

class FilamentExtruder
{
  public:
    FilamentExtruder(MotorDefinition pullerMotorDef);
    ~FilamentExtruder();

    void setup();
    void stop();

    void startAction(Message* msg);
    LoopStates loop();

    inline float getPullerRPM() __attribute__((always_inline)) {
        return _pullerRPM;
    };

    inline void setPullerRPM(float rpm) __attribute__((always_inline)) {
        _pullerRPM = rpm;
        _pullerMotorStepper->setRPM(_pullerRPM);
    };

    inline FilamentExtruderStates getFilamentExtruderState() __attribute__((always_inline)) {
      return _currentState;
    };

  private:

    void changeState(FilamentExtruderStates newState);

    FilamentExtruderStates _currentState;
    MotorDefinition _pullerMotorDef;
    StepperMotor* _pullerMotorStepper;
    float _pullerRPM;

    // DEBUG
    IntervalTimer _intervalTimer;

};

#endif