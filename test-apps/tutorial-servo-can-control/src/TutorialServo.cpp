#include "TutorialServo.h"
#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs, float maxPulsewidthInMs) {
    this->m_servoPwmOut = new PwmOut(servoPin);
    this->m_servoRangeInDegrees = servoRangeInDegrees;
    this->m_minPulsewidthInMs = minPulsewidthInMs;
    this->m_maxPulsewidthInMs = maxPulsewidthInMs;
}

void TutorialServo::setPositionInDegrees(const float degrees) {
    m_servoPwmOut->pulsewidth(degrees / m_servoRangeInDegrees);
}

float TutorialServo::getServoRangeInDegrees() const {
    return this->m_servoRangeInDegrees;
}

float TutorialServo::getMinPulseWidthInMs() const{
    return this->m_minPulsewidthInMs;
}

float TutorialServo::getMaxPulseWidthInMs() const {
    return this->m_maxPulsewidthInMs;
}