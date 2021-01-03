#include "TutorialServo.h"

#include <cstdlib>

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1,
                             float maxPulsewidthInMs = 2) {
  m_servoRangeInDegrees = servoRangeInDegrees;
  m_minPulsewidthInMs   = minPulsewidthInMs;
  m_maxPulsewidthInMs   = maxPulsewidthInMs;
  m_servoPwmOut(servoPin);
  m_servoPwmOut.period(0.020);
}

float TutorialServo::getServoRangeInDegrees() const {
  return m_servoRangeInDegrees;
}

float TutorialServo::getMinPulseWidthInMs() const {
  return m_minPulsewidthInMs;
}

float TutorialServo::getMaxPulseWidthInMs() const {
  return m_maxPulsewidthInMs;
}

void TutorialServo::setPositionInDegrees(const float degrees) {
  if (degrees >= 0 && degrees <= m_servoRangeInDegrees) {
    m_servoPwmOut.pulsewidth_ms((degrees / m_servoRangeInDegrees) * (m_maxPulsewidthInMs - m_minPulsewidthInMs) +
                                m_minPulsewidthInMs);
  }
}