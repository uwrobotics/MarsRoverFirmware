#pragma once
#include "mbed.h"

class TutorialServo{

  private:

        PwmOut m_servoPwmOut;
        float m_servoRangeInDegrees;
        float m_minPulsewidthInMs;
        float m_maxPulsewidthInMs;

  public:

        TutorialServo(PinName servoPin, float servoRangeInDegrees = 180, float minPulseInMs = 1, float maxPulsewidthInMs = 2);
        void setAngleRangeInDegrees(float degrees);

        void setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs);
        void setPositionInDegrees(float degrees);
        float getRangeInDegrees();

};

