//
//  TutorialServo.cpp
//  
//
//  Created by Ari Wasch on 9/18/20.
//

#include "TutorialServo.h"
#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs, 
                            float maxPulsewidthInMs) {
  m_servoPwmOut = m_servoPwmOut(servoPin);
  m_servoPwmOut.period(0.020);
  m_servoRangeInDegrees = servoRangeInDegrees;
  m_minPulsewidthInMs = minPulsewidthInMs;
  m_maxPulsewidthInMs = maxPulsewidthInMs;
}
 
void TutorialServo::setAngleRangeInDegrees(float degrees) {
  m_servoRangeInDegrees = degrees;
}
void TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs) {
  m_minPulsewidthInMs = minPulsewidthMs;
  m_maxPulsewidthInMs = maxPulsewidthMs;
}

void TutorialServo::setPositionInDegrees(float degrees) {
  if(degrees >= 0 && degrees <= m_servoRangeInDegrees){
    m_servoPwmOut.pulsewidth_ms((degrees/m_servoRangeInDegrees) * (m_maxPulsewidthInMs - m_minPulsewidthInMs) + 
    m_minPulsewidthInMs);
  }
}

float TutorialServo::getServoRangeInDegrees()const { 
  return m_servoRangeInDegrees;
}
   // Get the min pulse width in ms (ex: 1ms)
float TutorialServo::getMinPulseWidthInMs() const {
  return m_minPulsewidthInMs;
}
   // Get the max pulse width in ms (ex: 2ms)
float TutorialServo::getMaxPulseWidthInMs() const {
  return m_maxPulsewidthInMs;
}
 
// void TutorialServo::goCrazy(int n) {
//     for(int i=0; i<n*2; i++) {
//         _pin = !_pin;
//         wait(0.2);
//     }
// }
