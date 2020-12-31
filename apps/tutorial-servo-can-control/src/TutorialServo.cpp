#include "TutorialServo.h"

#include "mbed.h"

//CONSTRUCTOR
TutorialServo :: TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0,
    float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2)
{
    PwmOut servoPwmOut(servoPin);
    m_servoPwmOut = servoPwmOut; 

    m_servoRangeInDegrees = servoRangeInDegrees;
    m_minPulsewidthInMs = minPulsewidthInMs;
    m_maxPulsewidthInMs = maxPulsewidthInMs;
}

// Set servo position (ex. 45 deg) --> assume servo can't make continuous motion
void TutorialServo :: setPositionInDegrees(const float degrees)
{
    //check bounds on degrees. modify if out of bounds
    float newPosition = m_minPulsewidthInMs + m_minPulsewidthInMs*(degrees/m_servoRangeInDegrees);
    if(newPosition > m_maxPulsewidthInMs)
        newPosition = m_maxPulsewidthInMs;
    else if(newPosition < m_minPulsewidthInMs)
        newPosition = m_minPulsewidthInMs;

    m_servoPwmOut.pulsewidth(newPosition);
}

//ACCESSORS
// Get the servo range in degrees
float TutorialServo :: getServoRangeInDegrees( ) const
{
    return m_servoRangeInDegrees;
}
// Get the min pulse width in ms
float TutorialServo :: getMinPulseWidthInMs( ) const
{
    return m_minPulsewidthInMs;
}
// Get the max pulse width in ms
float TutorialServo :: getMaxPulseWidthInMs( ) const
{
    return m_maxPulsewidthInMs;
}