#include "TutorialServo.h"
#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs, float maxPulsewidthInMs) {
    this->m_servoPwmOut = new PwmOut(servoPin);
    this->m_servoPwmOut->period_ms(20);  //setting the PWM period to 20ms
    this->m_servoRangeInDegrees = servoRangeInDegrees;
    this->m_minPulsewidthInMs = minPulsewidthInMs;
    this->m_maxPulsewidthInMs = maxPulsewidthInMs;
}

void TutorialServo::setPositionInDegrees(const float degrees) {
    this->m_servoPwmOut->pulsewidth((1 + degrees/this->m_servoRangeInDegrees) / 1000);
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