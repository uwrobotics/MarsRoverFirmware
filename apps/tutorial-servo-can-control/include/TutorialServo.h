
#pragma once 

#include "mbed.h"

class TutorialServo{
	
	public:
	
		TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0,
					  float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2);
					  		  
		void setPositionInDegrees(const float degrees);
	
		float getServoRangeInDegrees() const;
		
		float getMinPulseWidthInMs() const;
	
		float getMaxPulseWidthInMs() const;
	
	
	private:
	
		PwmOut m_servoPwmOut;
		const float m_servoRangeInDegrees;
		const float m_minPulsewidthInMs;
		const float m_maxPulsewidthInMs;
		
	
};