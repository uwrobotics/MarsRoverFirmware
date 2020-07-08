#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs): m_servoPwmOut(servoPin),
                              m_servoRangeInDegrees(servoRangeInDegrees), 
                              m_minPulsewidthInMs(minPulsewidthInMs),
                              m_maxPulsewidthInMs(maxPulsewidthInMs) {
  // do nothing
}

TutorialServo::TutorialServo(PinName servoPin) : m_servoPwmOut(servoPin){
  m_servoRangeInDegrees = 180;
  m_minPulsewidthInMs = 1;
  m_maxPulsewidthInMs = 2;
}

TutorialServo::~TutorialServo() {
  m_servoRangeInDegrees = 0;
  m_minPulsewidthInMs   = 0;
  m_maxPulsewidthInMs   = 0;
}

void TutorialServo::setAngleRangeInDegrees(float degrees) {
  m_servoRangeInDegrees = degrees;
}

void TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs) {
  m_minPulsewidthInMs = minPulsewidthMs;
  m_maxPulsewidthInMs = maxPulsewidthMs;
  //m_servoPwmOut.period((maxPulsewidthMs - minPulsewidthMs) / 1000);
}

void TutorialServo::setPositionInDegrees(float degrees) {}
