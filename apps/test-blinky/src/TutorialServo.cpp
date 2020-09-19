//
//  TutorialServo.cpp
//  
//
//  Created by Ari Wasch on 9/18/20.
//

#include "TutorialServo.h"
#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2) : _pin(pin) {
    _pin = 0;
    m_servoPwmOut(_pin)
    m_servoPwmOut.period(0.020);
    m_servoRangeInDegrees = servoRangeInDegrees
    m_minPulsewidthInMs = minPulsewidthInMs
    m_maxPulsewidthInMs = maxPulsewidthInMs

}
 
void TutorialServo::setAngleRangeInDegrees(float degrees) {
    m_servoRangeInDegrees = servoRangeInDegrees
}
void TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs) {
    m_minPulsewidthInMs = minPulsewidthInMs
    m_maxPulsewidthInMs = maxPulsewidthInMs

}

void setPositionInDegrees(float degrees) {
    if(degrees >= 0 && degrees <= servoRangeInDegrees){
        m_servoPwmOut.pulsewidth((degrees/servoRangeInDegrees) * (m_maxPulsewidthInMs - m_minPulsewidthInMs) + m_minPulsewidthInMs)
    }

}
 
void TutorialServo::goCrazy(int n) {
    for(int i=0; i<n*2; i++) {
        _pin = !_pin;
        wait(0.2);
    }
}
