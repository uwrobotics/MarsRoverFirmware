#include "TutorialServo.h"
#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs, float maxPulsewidthInMs): m_servoPwmOut(servoPin), m_servoRangeInDegrees(servoRangeInDegrees), m_minPulsewidthInMs(minPulsewidthInMs), m_maxPulsewidthInMs(maxPulsewidthInMs) {
        m_servoPwmOut.period_ms(20);
}

float TutorialServo::getServoRangeInDegrees( ) const{
    return m_servoRangeInDegrees;
}

float TutorialServo::getMinPulseWidthInMs( ) const{
    return m_minPulsewidthInMs;
}

float TutorialServo::getMaxPulseWidthInMs( ) const{
    return m_maxPulsewidthInMs;
}

void TutorialServo::setPositionInDegrees(const float degrees){
    if (degrees < 0){
        /*Out of Range. Setting to min*/
        m_servoPwmOut.pulsewidth(m_minPulsewidthInMs * 0.001);
        return;
    }
    else if(degrees > m_servoRangeInDegrees){
        /*Out of Range. Setting to max*/
        m_servoPwmOut.pulsewidth(m_maxPulsewidthInMs * 0.001);
        return;
    }
    float slope = (m_maxPulsewidthInMs - m_minPulsewidthInMs)/m_servoRangeInDegrees;
    float pulse = (m_minPulsewidthInMs + degrees*slope) * 0.001;
    /*Setting to degrees*/
    m_servoPwmOut.pulsewidth(pulse);
    return;
}