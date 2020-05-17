#include "mbed.h"
#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, 
                            float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2):
            m_servoPwmOut(servoPin), 
            m_servoRangeInDegrees(servoRangeInDegrees),
            m_minPulsewidthInMs(minPulsewidthInMs),
            m_maxPulsewidthInMs(maxPulsewidthInMs){
                m_servoPwmOut = 0;
            }

TutorialServo::setAngleRangeInDegrees(float degrees){
    this->servoRangeInDegrees = degrees
}

TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs){
    this->m_minPulsewidthInMs = minPulsewidthMs;
    this->m_maxPulsewidthInMs = maxPulsewidthMs;
}

TutorialServo::setPositionInDegrees(float degrees){
    m_servoPwmOut.pulsewidth(
        ((this->m_maxPulsewidthInMs - this->m_minPulsewidthInMs)/(max_a)) * degrees + this->m_minPulsewidthInMs
        );

}

TutorialServo::setPositionInPercent(float percent){
    this->setPositionInDegrees(percent * this->m_servoRangeInDegrees);
}