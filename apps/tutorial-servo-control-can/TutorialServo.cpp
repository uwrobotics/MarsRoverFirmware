//TutorialServo Source file

#include "mbed.h"
#include "TutorialServo.h"


// Constructor: Takes a servo pin name (ex. PA_1), and optionally a servo range
// that has a default value of 180.0 degrees, a minimum pulsewidth of 1ms, and a     
// maximum pulsewidth of 2ms.
TutorialServo :: TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs, float maxPulsewidthInMs)
	:m_servoPwmOut(servoPin), m_servoRangeInDegrees(servoRangeInDegrees), 
	   m_minPulsewidthInMs(minPulsewidthInMs), m_maxPulsewidthInMs(maxPulsewidthInMs); //sevoPin like PA_1
{
	m_servoPwmOut.period(0.02); //call func to set period on m_servo
}

// Set servo range (ex. 270 deg)
void TutorialServo :: setAngleRangeInDegrees(float degrees)
{
	m_servoRangeInDegrees = degrees;
}

// Set servo pulse width range (ex. 1ms, 2ms)
void TutorialServo :: setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs)
{
	m_minPulsewidthInMs = minPulsewidthMs;
	m_maxPulsewidthInMs = maxPulsewidthMs;
}

// Set servo position (ex. 45 deg)
void TutorialServo :: setPositionInDegrees(float degrees)
{
	//need % of range, use min and max pw
	float pulseRange = maxPulsewidthInMs - minPulsewidthInMs;
	
	m_servoPwmOut.pulsewidth(minPulsewidthInMs + (degrees/m_servoRangeInDegrees *pulseRange));
}


