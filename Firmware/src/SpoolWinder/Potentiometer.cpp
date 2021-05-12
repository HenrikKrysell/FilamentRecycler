#include "Potentiometer.h"

Potentiometer::Potentiometer(int potentiometerPin, int pollIntervalMs)
{
  _potentiometerPin = potentiometerPin;
  _currentValue = 0;
  _intervalTimer.setInterval(pollIntervalMs);
}
 
 Potentiometer::~Potentiometer()
 {
 }

void Potentiometer::setup()
{
  pinMode(_potentiometerPin, INPUT);
}

int Potentiometer::fetchPosition()
{
  if (_intervalTimer.isTriggered())
    _currentValue = analogRead(_potentiometerPin);

  return _currentValue;
}

