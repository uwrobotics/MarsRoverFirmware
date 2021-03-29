
#include "TutorialServo.h"
#include "mbed.h"


	
TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0,
							 float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2){
		m_servoPwmOut = servoPin;
		m_servoPwmOut.period_ms(20);
		m_servoRangeInDegrees = servoRangeInDegrees;
		m_minPulsewidthInMs = minPulsewidthInMs;
		m_maxPulsewidthInMs = maxPulsewidthInMs;						 
	
}
						
void TutorialServo::setPositionInDegrees(const float degrees){

	float offset;

	if (degrees < -m_servoRangeInDegrees) offset = -m_servoRangeInDegrees; 
	else if (degrees > m_servoRangeInDegrees) offset = m_servoRangeInDegrees;
	else offset = degrees/m_servoRangeInDegrees;

    m_servoPwmOut.pulsewidth_ms(1 + offset);
}

float TutorialServo::getServoRangeInDegrees() const{

	return m_servoRangeInDegrees;
}

float TutorialServo::getMinPulseWidthInMs() const{

	return m_minPulsewidthInMs;
}

float TutorialServo::getMaxPulseWidthInMs() const{

	return m_maxPulsewidthInMs;
}		
	
};