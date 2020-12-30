#include "Tutorialservo.h"
#include <cstdlib>

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2){
    m_servoRangeInDegrees = servoRangeInDegrees;
    m_minPulsewidthInMs = minPulsewidthInMs;
    m_maxPulsewidthInMs = maxPulsewidthInMs;
    m_servoPwmOut (servoPin);
    m_servoPwmOut.period(0.020);
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
     /*if(!((std::fabs(degrees) > angleRange)))
    {
        double pulseWidthMs = 1.5 + 0.5*degrees/angleRange; //Sets the pulse width that will be used, as 1ms is the lower bound and 2ms is the upper bound.
        //1.5 is neutral and degrees can be either positive or negative which can make a pulse with from 1-2ms
        if((minPulseWidthMs < pulseWidthMs) && (pulseWidthMs < maxPulseWidthMs)) //checks to ensure it is within bounds
        {
           servoPwm.pulsewidth(pulseWidthMs/1000); //set pulse width
        }     

    }*/

}


