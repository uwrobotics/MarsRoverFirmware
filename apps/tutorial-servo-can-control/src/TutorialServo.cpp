#include "TutorialServo.h"
#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs, float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin), m_servoRangeInDegrees(servoRangeInDegrees), m_minPulsewidthInMs(minPulsewidthInMs), m_maxPulsewidthInMs(maxPulsewidthInMs) 
    {}
       
// Set servo position (ex. 45 deg)
void TutorialServo::setPositionInDegrees(const float degrees) {
    m_servoPwmOut.period_ms(20);
    m_servoPwmOut.pulsewidth_ms(m_minPulsewidthInMs + degrees/m_servoRangeInDegrees);
}

// Get the servo range in degrees (ex: 90 deg)
float TutorialServo::getServoRangeInDegrees( ) const {
    return m_servoRangeInDegrees;
}

// Get the min pulse width in ms (ex: 1ms)
float TutorialServo::getMinPulseWidthInMs( ) const {
    return m_minPulsewidthInMs;
}

// Get the max pulse width in ms (ex: 2ms)
float TutorialServo::getMaxPulseWidthInMs( ) const {
    return m_maxPulsewidthInMs;
}


