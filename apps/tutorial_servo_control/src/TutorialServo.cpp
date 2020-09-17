#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs , float maxPulsewidthInMs){
        m_servoRangeInDegrees =  servoRangeInDegrees;
        m_minPulsewidthInMs = minPulsewidthInMs;
	m_maxPulsewidthInMs = maxPulsewidthInMs;
}

float TutorialServo::getServoRangeInDegrees() const{

        return (m_servoRangeInDegrees);
}

float TutorialServo::getMinPulseWidthInMs()const{

        return (m_minPulsewidthInMs);

}
float TutorialServo::getMaxPulseWidthInMs()const{

        return (m_maxPulsewidthInMs);

}
void TutorialServo::setPositionInDegrees(float degrees){

         float slope{0.0}, pulse{0.0}, range{m_maxPulsewidthInMs - m_minPulsewidthInMs};
         slope = (m_maxPulsewidthInMs - m_minPulsewidthInMs)/(m_servoRangeInDegrees*1000);
         pulse = (m_minPulsewidthInMs/1000) + (range*degrees)/slope;
         m_servoPwmOut.pulsewidth(pulse);
}

