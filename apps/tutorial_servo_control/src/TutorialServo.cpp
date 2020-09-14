#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180, float minPulseInMs = 1, float maxPulsewidthInMs = 2){
        m_servoRangeInDegrees =  servoRangeInDegrees;
        m_minPulsewidthInMs = minPulsewidthInMs;
	m_maxPulsewidthInMs = maxPulsewidthInMs;
}

void TutorialServo::setAngleRangeInDegrees(float degrees){

        m_servoRangeInDegrees = degrees;
}

void TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs){

        m_minPulsewidthInMs = minPulsewidthMs;
        m_maxPulsewidthInMs = maxPulsewidthMs;
}

void TutorialServo::setPositionInDegrees(float degrees){

         float slope{0.0}, pulse{0.0};
         slope = (m_maxPulsewidthInMs - m_minPulsewidthInMs)/(m_servoRangeInDegrees*1000);
         pulse = (m_minPulsewidthInMs/1000) + degrees/slope;
         m_servoPwmOut.pulsewidth(pulse);

}

float TutorialServo::getRangeInDegrees(){

         return m_servoRangeInDegrees;
}
