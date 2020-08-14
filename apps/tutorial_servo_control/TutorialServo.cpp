#include "TutorialServo.h"
#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1,
                             float maxPulsewidthInMs = 2) 
	: _servoPwmOut(servoPin),
      _minPulseWidthInMs(minPulsewidthInMs),
      _maxPulseWidthInMs(maxPulsewidthInMs),
      _servoRangeInDegrees(servoRangeInDegrees) 
{
	_minPulseWidthInMs = _minPulseWidthInMs / 1000;
	_maxPulseWidthInMs = _maxPulseWidthInMs / 1000;
	//default period of servo is 50Hz or 20 milliseconds 
	_servoPwmOut.period(0.020);
}

void TutorialServo::setAngleRangeInDegrees(float degrees) {
	_servoRangeInDegrees = degrees; 
}

void TutorialServo::setPulseWidthRangeInMs(float minPulseWidthInMs, float maxPulseWidthInMs) {
	_minPulseWidthInMs = minPulseWidthInMs/1000;
	_maxPulseWidthInMs = maxPulseWidthInMs/1000;
}
void TutorialServo::setServoPositionInDegrees(float degrees) {
	if (degrees > 180) {
		degrees = 180;
	}
	float value;
	value = ((_maxPulseWidthInMs - _minPulseWidthInMs) / _servoRangeInDegrees) * degrees) + _minPulseWidthInMs;
	_servoPwmOut.pulsewidth(value);
}

void TutorialServo::setServoPositionWithPercent(float percent) {
  if (percent > 100) {
    percent = 100;
  }
  float value;
  value = ((_maxPulseWidthInMs - _minPulseWidthInMs) / 100) * percent) + _minPulseWidthInMs;
  _servoPWmOut.pulsewidth(value);

}
