#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1,
                             float maxPulsewidthInMs = 2): m_servoPwmOut(servoPin),
                              m_servoRangeInDegrees(servoRangeInDegrees), 
                              m_minPulsewidthInMs(minPulsewidthInMs),
                              m_maxPulsewidthInMs(maxPulsewidthInMs) {
  // do nothing
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
  //m_servoPmwOut.period((maxPulsewidthMs - minPulsewidthMs) / 1000);
}

void TutorialServo::setPositionInDegrees(float degrees) {}
