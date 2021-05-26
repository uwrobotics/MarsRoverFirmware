
#include "mbed.h"
#include "TutorialServo.h"

#include <iostream>
using namespace std;


TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0,
float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2)
{
    //declares this pin as pwm using pwmout.
    PwmOut servoPwmOut(servoPin);

    //set the PWM period for that pin to 20ms
    servoPwmOut.period_ms(20);


    while(1)
    {
        servoPwmOut.pulsewidth_ms(minPulsewidthInMs);
        servoPwmOut.pulsewidth_ms(maxPulsewidthInMs);
    }
}

void TutorialServo::setPositionInDegrees(const float degrees)
{
   
    m_servoRangeInDegrees = degrees;
}

float TutorialServo::getServoRangeInDegrees() const
{
    return m_servoRangeInDegrees;
}

float TutorialServo::getMinPulseWidthInMs() const
{
    return m_minPulsewidthInMs;
}

float TutorialServo::getMaxPulseWidthInMs() const
{
    return m_maxPulsewidthInMs;
}