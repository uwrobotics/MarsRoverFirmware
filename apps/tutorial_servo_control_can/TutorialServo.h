#pragma once
#include "mbed.h"
class TutorialServo
{
public:
	//default servo settings 
	TutorialServo(PinName servoPin);
	//these functions change an aspect of the servo if need be
	void setAngleRangeInDegrees(float degrees);
	void setPulseWidthRangeInMs(float minPulseWidthInMs, float maxPulseWidthInMs);
	void setServoPositionInDegrees(float degrees);

private:
	PwmOut _servoPwmOut;
    float _minPulseWidthInMs;
	float _maxPulseWidthInMs;
    float _servoRangeInDegrees;
};

