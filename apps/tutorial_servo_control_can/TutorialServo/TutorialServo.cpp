#include "./TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, 
                            float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2):
            m_servoPwmOut(servoPin), 
            m_servoRangeInDegrees(servoRangeInDegrees),
            m_minPulsewidthInMs(minPulsewidthInMs),
            m_maxPulsewidthInMs(maxPulsewidthInMs){
            }

void TutorialServo::setAngleRangeInDegrees(float degrees){
    this->m_servoRangeInDegrees = degrees;
}

void TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs){
    this->m_minPulsewidthInMs = minPulsewidthMs;
    this->m_maxPulsewidthInMs = maxPulsewidthMs;
}

void TutorialServo::setPositionInDegrees(float degrees){
    m_servoPwmOut.pulsewidth(
        ((this->m_maxPulsewidthInMs - this->m_minPulsewidthInMs)/(2 * this->m_servoRangeInDegrees)) * degrees + 
        ((this->m_maxPulsewidthInMs + this->m_minPulsewidthInMs) / 2.0)
        );
}

void TutorialServo::setPositionInPercent(float percent){
    this->setPositionInDegrees(percent * this->m_servoRangeInDegrees);
}