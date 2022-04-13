#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, 
    float servoRangeInDegrees = 180.0, 
    float minPulsewidthInMs = 1, 
    float maxPulsewidthInMs = 2): 
    m_servoPwmOut(servoPin),
    m_servoRangeInDegrees(servoRangeInDegrees),
    m_minPulsewidthInMs(minPulsewidthInMs)
    m_maxPulsewidthInMs(maxPulsewidthInMs){
        //period depends on the servo itself, check datasheet
		servoPwnOut.period_ms(20);
}

void TutorialServo::setPositionInDegrees(const float degrees){
    //assume degress are always < 360

    //neglect the case where desire degree to set to is greater than the range
    if(degrees > m_servoRangeInDegrees) return;

    //ratio of degrees/range = pulseWidth/(maxPulseWidth - minPulseWidth)
    //map the desire degree to its corresponding pulseWidth according to the ratio above
    m_servoPwmOut.pulsewidth( ( m_minPulsewidthInMs + (degrees/m_servoRangeInDegrees)*(m_maxPulsewidthInMs - m_minPulsewidthInMs) ) / 1000 );
    
}


float TutorialServo::getServoRangeInDegrees( ) const {
    return this -> m_servoRangeInDegrees;
}

float TutorialServo::getMinPulseWidthInMs( ) const {
    return this -> m_minPulsewidthInMs;
}
        
float TutorialServo::getMaxPulseWidthInMs( ) const {
    return this -> m_maxPulsewidthInMs;
}