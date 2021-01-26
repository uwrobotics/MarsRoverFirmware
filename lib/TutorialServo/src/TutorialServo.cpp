#include "TutorialServo.h"
#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, 
    float minPulsewidthInMs, float maxPulsewidthInMs):
    m_servoPwmOut(servoPin), 
    m_servoRangeInDegrees(servoRangeInDegrees), 
    m_minPulsewidthInMs(minPulsewidthInMs), 
    m_maxPulsewidthInMs(maxPulsewidthInMs){}

void TutorialServo::setPositionInDegrees(float degrees){
    float PulsewidthRangeInMs = m_maxPulsewidthInMs - m_minPulsewidthInMs;
    float new_pulse_width = (degrees/m_servoRangeInDegrees)*PulsewidthRangeInMs + m_minPulsewidthInMs;
    m_servoPwmOut.pulsewidth(new_pulse_width);
}

float TutorialServo::getServoRangeInDegrees( ) const{
    return m_servoRangeInDegrees;l
}
float TutorialServo::getMinPulseWidthInMs( ) const{
    return m_minPulsewidthInMs;
}
float TutorialServo::getMaxPulseWidthInMs( ) const{
    return m_maxPulsewidthInMs;
}