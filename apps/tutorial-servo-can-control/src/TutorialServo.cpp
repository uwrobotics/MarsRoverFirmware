#include "TutorialServo.h"
#include <cmath>

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs, float maxPulsewidthInMs)
              : m_servoRangeInDegrees(servoRangeInDegrees), m_minPulsewidthInMs(minPulsewidthInMs), m_maxPulsewidthInMs(maxPulsewidthInMs)
{
    // Initialize servo pin and period
    m_servoPwmOut(servoPin);
    m_servoPwmOut.period_ms(m_periodInMs);

}

// Set servo position (ex. 45deg)
void TutorialServo::setPositionInDegrees(const float degrees){
    float finalPulsewidth = (((fabs(degrees) / m_servoRangeInDegrees) - floor(fabs(degrees)/m_servoRangeInDegrees)) * (m_maxPulsewidthInMs-m_minPulsewidthInMs)) + m_minPulsewidthInMs;
    m_servoPwmOut.pulsewidth_ms(finalPulsewidth);
}

 void TutorialServo::setPositionInPercentage(const float percentage){
     float finalPulseWidth = (percentage * (m_maxPulsewidthInMs-m_minPulsewidthInMs)) + m_minPulsewidthInMs;
     m_servoPwmOut.pulsewidth_ms(finalPulsewidth);
 }

// Get the servo range in degrees (ex. 90deg)
float TutorialServo::getServoRangeInDegrees() const{
    return m_servoRangeInDegrees;
}
(fabs(degrees) / m_servoRangeInDegrees) - floor(fabs(degrees)/m_servoRangeInDegrees)
// Get the min pulse width in ms (ex: 1ms)
float TutorialServo::getMinPulseWidthInMs() const{
    return m_minPulsewidthInMs;
}

// Get the max pulse width in ms (ex: 2ms)
float TutorialServo::getMaxPulseWidthInMs() const{
    return m_maxPulsewidthInMs;
}

