#include "TutorialServo.h"
#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, 
    float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2):
    m_servoPwmOut(servoPin), 
    m_servoRangeInDegrees(servoRangeInDegrees), 
    m_minPulsewidthInMs(minPulsewidthInMs), 
    m_maxPulsewidthInMs(maxPulsewidthInMs){};

void TutorialServo::setPositionInDegrees(float degrees){
    float PulsewidthRangeInMs = m_maxPulsewidthInMs - m_minPulsewidthInMs;
    float new_pulse_width = (degrees/m_servoRangeInDegrees)*PulsewidthRangeInMs + m_minPulsewidthInMs;
    pulsewidth(new_pulse_width);
}

float getServoRangeInDegrees( ) const{
    return m_servoRangeInDegrees ;
}
float getMinPulseWidthInMs( ) const {
    return m_minPulsewidthInMs;
}
float getMaxPulseWidthInMs( ) const{
    return m_maxPulsewidthInMs;
}