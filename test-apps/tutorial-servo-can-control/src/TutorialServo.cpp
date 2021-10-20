#include "TutorialServo.h"
#include "mbed.h"

//I believe we are writing to servoPin, which I set to be PA_1
CAN can1(PA_0, servoPin);
float pulseWidthToSend;

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2):m_servoPwmOut(servoPin), m_servoRangeInDegrees(servoRangeInDegrees), m_minPulsewidthInMs(minPulsewidthInMs), m_maxPulsewidthInMs(maxPulsewidthInMs) {}

// Set servo position (ex. 45 deg)
void setPositionInDegrees( const float degrees) {
    m_servoPwmOut.pulsewidth((1+degrees/servoRangeInDegrees) / 1000);
}

// Get the servo range in degrees (ex: 90 deg)
float getServoRangeInDegrees( ) const {
    return m_servoRangeInDegrees;
}

// Get the min pulse width in ms (ex: 1ms)
float getMinPulseWidthInMs( ) const {
    return m_minPulsewidthInMs;
}

// Get the max pulse width in ms (ex: 2ms)
float getMaxPulseWidthInMs( ) const {
    return m_maxPulsewidthInMs;
}