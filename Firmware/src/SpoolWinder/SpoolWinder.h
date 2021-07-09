#ifndef SpoolWinder_h
#define SpoolWinder_h

#include <Arduino.h>
#include "../DataStructures/motorDefinition.h"
#include "../Utilities/StepperHomingHelper.h"
#include <A4988.h>
#include "../Utilities/Constants.h"
#include "../Drivers/StepperMotor.h"
#include "Potentiometer.h"
#include "../Utilities/IntervalTimer.h"
#include "../Utilities/LoopState.h"
#include "../CommunicationProtocol/Incomming/IncommingMessage.h"
#include "../CommunicationProtocol/Incomming/PerformAction.h"

enum class SpoolWinderStates {
  Idle = 0,
  Stop,
  ProductionSpool,
  ProductionWaitForSensor,
  MoveAxis,
  HomeAllAxes,
  UnwindSpool,
};
const static char* SpoolWinderStatesNames[] = { "0:Idle", "1:Stopped", "2:SpoolSlow", "3:SpoolFast" };

class SpoolWinder
{
  public:
    SpoolWinder(MotorDefinition spoolWinderMotorDef, MotorDefinition filamentGuideMotorDef, int filamentGuideEndStopPin, int tensionerPotentiometerPin);
    ~SpoolWinder();
    
    void setup();
    void stop();

    void startAction(IncommingMessage* msg);
    LoopStates loop();

    inline SpoolWinderStates getState() __attribute__((always_inline)) {
        return _currentState;
    };

    inline long getFilamentGuidePosition() __attribute__((always_inline)) {
        return _filamentGuideStepper->getPosition();
    };

    inline long getFilamentGuideStartPos() __attribute__((always_inline)) {
        return _filamentGuideStartPos;
    };

    inline long getFilamentGuideStopPos() __attribute__((always_inline)) {
        return _filamentGuideStopPos;
    };

    inline int getFilamentSpoolDiameter() __attribute__((always_inline)) {
        return _filamentSpoolDiameter;
    };

    inline int getFilamentSpoolWidth() __attribute__((always_inline)) {
        return _filamentSpoolWidth;
    };

    inline void setFilamentGuideStartPos(long absolutePosition) __attribute__((always_inline)) {
        _filamentGuideStartPos = absolutePosition;
    };

    inline void setFilamentGuideStopPos(long absolutePosition) __attribute__((always_inline)) {
        _filamentGuideStopPos = absolutePosition;
        calculateNewGuideStepsPerSpoolRevolution();
    };

    inline void setFilamentSpoolDiameter(int value) __attribute__((always_inline)) {
        _filamentSpoolDiameter = value;
    };

    inline void setFilamentSpoolWidth(int value) __attribute__((always_inline)) {
        _filamentSpoolWidth = value;
        calculateNewGuideStepsPerSpoolRevolution();
    };

    inline void setSpoolWinderGearRatio(int value) __attribute__((always_inline)) {
        _spoolWinderGearRatio = value;
        _spoolWinderNumStepsPerRevolution = (unsigned long)_spoolWinderGearRatio * _spoolWinderStepper->getSteps() * _spoolWinderStepper->getMicrostep();
    };

    inline int getSpoolWinderGearRatio() __attribute__((always_inline)) {
        return _spoolWinderGearRatio;
    };

    inline int getFilamentGuideStepsPerSpoolRevolution() __attribute__((always_inline)) {
        return _filamentGuideStepsPerSpoolRevolution;
    };

    inline long getSpoolWinderNumStepsPerRevolution() __attribute__((always_inline)) {
        return _spoolWinderNumStepsPerRevolution;
    };

  private:

    inline void calculateNewGuideStepsPerSpoolRevolution() __attribute__((always_inline)) {
        long totalSteps = abs(_filamentGuideStopPos - _filamentGuideStartPos);
        if (_filamentSpoolWidth == 0)
            _filamentGuideStepsPerSpoolRevolution = 0;
        else {
            _filamentGuideStepsPerSpoolRevolution = (long)ceil((totalSteps / (float)_filamentSpoolWidth)*(TARGET_FILAMENT_DIAMETER + 0.05f));
            // Serial.print("_filamentGuideStepsPerSpoolRevolution: ");
            // Serial.println(_filamentGuideStepsPerSpoolRevolution);
            _numRevolutionsPerLayer = floorf(_filamentSpoolWidth / TARGET_FILAMENT_DIAMETER) - 1;
            long _filamentGuideStepsPerSpoolRevolution = totalSteps / _numRevolutionsPerLayer;
            Serial.print("_filamentGuideStepsPerSpoolRevolution: ");
            Serial.println(_filamentGuideStepsPerSpoolRevolution);
            Serial.print("numRevolutionsPerLayer: ");
            Serial.println(_numRevolutionsPerLayer);
        }
            
    };


    void changeState(SpoolWinderStates newState);
    void getAxesValuesFromMessage(IncommingMessage* msg, long &spoolWinderPosition, long &filamentGuidePosition);

    MotorDefinition _spoolWinderMotorDef;
    MotorDefinition _filamentGuideMotorDef;
    int _filamentGuideEndStopPin;
    int _tensionerPotentiometerPin;
    bool _isWinding;
    bool _isHoming;
    bool _homingDone;
    StepperMotor* _spoolWinderStepper;
    StepperMotor* _filamentGuideStepper;
    StepperHomingHelper* _filemanetGuideHomingHelper;
    SpoolWinderStates _currentState;
    Potentiometer* _potentiometer;
    long _filamentGuideStartPos;
    long _filamentGuideStopPos;
    int _filamentSpoolDiameter;
    int _filamentSpoolWidth;
    int _spoolWinderGearRatio;
    unsigned long _spoolWinderNumStepsPerRevolution;
    long _filamentGuideStepsPerSpoolRevolution;
    int _filamentGuideDirection;

    int _numRevolutionsPerLayer;
    int _numRevolutionsThisLayer;
    int _numLayers;

    long _lastStepCountGuideMoved;
};

#endif