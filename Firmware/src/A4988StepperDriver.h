#ifndef A4988StepperDriver_h
#define A4988StepperDriver_h

#include <Arduino.h>
#include "MotorRotationDirection.h"

#define STEP_PULSE(steps, microsteps, rpm) (60.0*1000000L/steps/microsteps/rpm)

class A4988StepperDriver
{
  public:
    void init(int dirPin, int stepPin, int enablePin) {
        _dirPin = dirPin;
        _stepPin = stepPin;
        _enablePin = enablePin;
        _rotationDir = MotorRotationDirection::STOP;

        _currentRPM = 0;
        _running = false;
        pinMode(_dirPin, OUTPUT);
        pinMode(_stepPin, OUTPUT);
        pinMode(_enablePin, OUTPUT);
    }

    void setClockwiseRotation() {
        this->_rotationDir = MotorRotationDirection::CLOCKWISE;
        digitalWrite(_dirPin, LOW);
    }

    void setCounterClockwiseRotation() {
        this->_rotationDir = MotorRotationDirection::COUNTER_CLOCKWISE;
        digitalWrite(_dirPin, HIGH);
    }

    void stop() {
        _running = false;
        _lastPulseTime = 0;
        _currentRPM = 0;
    }

    void disableStepper() {
        _running = false;

        digitalWrite(_enablePin, LOW);
    }

    // For MKS Gen L, max rpm is around 600
    void setRPM(int rpm) {
        if (_currentRPM == rpm)
            return;
        if (rpm == 0) {
            stop();
            return;
        }

        digitalWrite(_enablePin, LOW);
        _running = true;
        _currentRPM = rpm;
        _delayBetweenPulsesInUs = STEP_PULSE(MOTOR_STEPS_PER_REV, MICROSTEPS, rpm);//calculateDelayBetweenPulses(_currentRPM);
        _lastPulseTime = 0;
    }

    void run() {
        if (_running && (micros() - _lastPulseTime >= _delayBetweenPulsesInUs)) {
             _lastPulseTime = micros();

            digitalWrite(_stepPin, HIGH);
            delayMicroseconds(MIN_PULSE_WIDTH_US);
            digitalWrite(_stepPin, LOW);
        }
    }

  private:
    const unsigned int MOTOR_STEPS_PER_REV = 200;
    const unsigned int MICROSTEPS = 16;
    const int MIN_PULSE_WIDTH_US = 2;

    int _dirPin;
    int _stepPin;
    int _enablePin;
    MotorRotationDirection _rotationDir;
    unsigned long _delayBetweenPulsesInUs;
    unsigned long _lastPulseTime;
    bool _running;

    int _currentRPM;
};

#endif