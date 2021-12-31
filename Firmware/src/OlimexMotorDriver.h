#ifndef OlimexMotorDriver_h
#define OlimexMotorDriver_h

#include <Arduino.h>
#include "MotorRotationDirection.h"

class OlimexMotorDriver
{
  public:
    void init(int inaPin, int inbPin, int pwmPin) {
        _inaPin = inaPin;
        _inbPin = inbPin;
        _pwmPin = pwmPin;

        pinMode(_inaPin, OUTPUT);
        pinMode(_inbPin, OUTPUT);
        pinMode(_pwmPin, OUTPUT);

        stop();

        #if defined(ARDUINO_AVR_MEGA2560)

        // https://www.etechnophiles.com/how-to-change-pwm-frequency-of-arduino-mega/
        // WARNING! DO NOT USE PIN 4 AND 13 WITH HIGH FREQUENCY WHEN USING STEPPER MOTORS, IT MESSES WITH THEM
        // if (_pwmPin == 4 || _pwmPin == 13)
        //     TCCR0B = TCCR0B & B11111000 | B00000010; // set PWM frequency of 7812.50 Hz for D4 and D13
        if (_pwmPin == 11 || _pwmPin == 12)
             TCCR1B = TCCR1B & B11111000 | B00000010;  // for PWM frequency of 3921.16 Hz
        if (_pwmPin == 9 || _pwmPin == 10)
             TCCR2B = TCCR2B & B11111000 | B00000010;  // for  PWM frequency of 3921.16 Hz
        if (_pwmPin == 2 || _pwmPin == 3 || _pwmPin == 5)
             TCCR3B = TCCR3B & B11111000 | B00000010;  // for PWM frequency of 3921.16 Hz

        #endif
    }

    void setRotation(MotorRotationDirection dir) {
        switch (dir)
        {
        case MotorRotationDirection::CLOCKWISE:
            setClockwiseRotation();
            break;
        case MotorRotationDirection::COUNTER_CLOCKWISE:
            setCounterClockwiseRotation();
            break;
        case MotorRotationDirection::STOP:
        default:
            stop();
            break;
        }
    }
    
    void stop() {
        _rotationDir = MotorRotationDirection::STOP;
        digitalWrite(_inaPin, LOW);
        digitalWrite(_inbPin, LOW);
    }

    void setClockwiseRotation() {
        this->_rotationDir = MotorRotationDirection::CLOCKWISE;
        digitalWrite(_inaPin, HIGH);
        digitalWrite(_inbPin, LOW);
    }

    void setCounterClockwiseRotation() {
        this->_rotationDir = MotorRotationDirection::COUNTER_CLOCKWISE;
        digitalWrite(_inaPin, LOW);
        digitalWrite(_inbPin, HIGH);
    }

    void setSpeed(byte speed) {
        if (_currentSpeed == speed)
            return;
        
        _currentSpeed = speed;
        analogWrite(_pwmPin, _currentSpeed);
    }

  private:
    int _inaPin;
    int _inbPin;
    int _pwmPin;
    MotorRotationDirection _rotationDir;

    byte _currentSpeed;
};

#endif