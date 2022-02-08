#ifndef AnalogSignalPoller_h
#define AnalogSignalPoller_h

#include <Arduino.h>

class AnalogSignalPoller
{
  public:
    void init(int analogPin, unsigned long pollIntervalMs, unsigned long delayBeforeFirstPollMs, int numSamples, unsigned long delayBetweenSamplesMs) {
        _analogPin = analogPin;
        _pollIntervalMs = pollIntervalMs;
        _numSamples = numSamples;
        _delayBetweenSamplesMs = delayBetweenSamplesMs;

        _accumulatedValue = 0;
        _numSamplesPerformed = 0;
        
        // TODO! These should generate an error instead!

        // pollIntervalMs must be greater than the total time it takes to polll
        if (_pollIntervalMs < numSamples*delayBetweenSamplesMs)
          _pollIntervalMs = numSamples*delayBetweenSamplesMs + 10;

        // delayBeforeFirstPollUs must be less than pollIntervalUs
        if (delayBeforeFirstPollMs > pollIntervalMs)
            delayBeforeFirstPollMs = pollIntervalMs;

        _lastPollTime = micros() - pollIntervalMs + delayBeforeFirstPollMs;
        pinMode(_analogPin, INPUT);

        _state = STATE::WAITING;
    }


    void run() {
      switch (_state)
      {
      case STATE::WAITING:
        if (millis() - _lastPollTime >= _pollIntervalMs) {
          _lastPollTime = millis();
          _accumulatedValue = 0;
          _numSamplesPerformed = 0;
          _state = STATE::POLLING;
          // make sure we trigger right away
          _lastSampleTime = millis() - _delayBetweenSamplesMs - 1;
        } else break;
      case STATE::POLLING:
        if (millis() - _lastSampleTime >= _delayBetweenSamplesMs) {
          _lastSampleTime = millis();
          _accumulatedValue += analogRead(_analogPin);
          _numSamplesPerformed++;
          if (_numSamplesPerformed >= _numSamples) {
            _state = STATE::WAITING;
            value = _accumulatedValue / _numSamplesPerformed;
          }
        }
        break;
      default:
        break;
      }
    }

    int getValue() { return value; }

  private:
    enum class STATE {
      POLLING,
      WAITING,
    };

    int _analogPin;
    unsigned long _pollIntervalMs;
    unsigned long _lastPollTime;
    int value;

    unsigned long _lastSampleTime;
    int _numSamples;
    unsigned long _delayBetweenSamplesMs;
    long _accumulatedValue;
    int _numSamplesPerformed;
    STATE _state;
};

#endif