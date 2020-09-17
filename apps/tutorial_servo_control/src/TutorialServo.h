#pragma once
#include "mbed.h"

class TutorialServo{

  private:

        PwmOut m_servoPwmOut;
        const float m_servoRangeInDegrees;
        const float m_minPulsewidthInMs;
        const float m_maxPulsewidthInMs;

  public:

        TutorialServo(PinName servoPin, float servoRangeInDegrees = 180, float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2);
        void setPositionInDegrees(const float degrees);
        float getMinPulseWidthInMs() const;
        float getMaxPulseWidthInMs() const;
	float getServoRangeInDegrees() const;
//	float getRangeInDegrees() const;

};

