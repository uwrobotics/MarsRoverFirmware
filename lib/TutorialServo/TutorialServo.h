#pragma once

class TutorialServo
{
public:
	//default servo settings 
	TutorialServo(PinName servoPin, float minPulseWidthInMs = 1, float maxPulseWidthInMs = 2,float servoRangeInDegrees = 180);     
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

