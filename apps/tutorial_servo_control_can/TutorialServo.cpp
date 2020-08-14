#include "TutorialServo.h"
#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin) 
	:_servoPwmOut(servoPin), _minPulseWidthInMs(1), _maxPulseWidthInMs(2), _servoRangeInDegrees(180)
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
	value = ((_maxPulseWidthInMs - _minPulseWidthInMs / _servoRangeInDegrees) * degrees) + _minPulseWidthInMs;
	_servoPwmOut.pulsewidth(value);
}
