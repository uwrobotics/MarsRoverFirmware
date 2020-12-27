#include "TutorialServo.cpp"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, 
                 float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2){
    m_servoPwmOut = servoPwmOut(sevrvoPin);
    m_servoRangeInDegrees = servoRangeInDegrees;
    m_minPulsewidthInMs = minPulsewidthInMs;
    m_maxPulsewidthInMs = maxPulsewidthInMs; 

}

void TutorialServo::setAngleRangeInDegrees(float degrees){
    m_servoRangeInDegrees = servoRangeInDegrees;
}                 

void TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs){
    m_minPulsewidthInMs = minPulsewidthInMs;
    m_maxPulsewidthInMs = maxPulsewidthInMs; 
}

void TutorialServo::setPositionInDegrees(float degrees)   {
    servoPwmOut.pulsewidth(.001+.001*degrees/servoRangeInDegrees);
}