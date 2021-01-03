#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {}

void TutorialServo::setPositionInDegrees(const float degrees) {
  // assumption negitive degrees is just ignored
  float n_degree = degrees;
  if (degrees < 0) {
    n_degree = 0;
  }
  if (n_degree > m_servoRangeInDegrees) {
    n_degree = m_servoRangeInDegrees;
  }
  m_servoPwmOut.pulsewidth((m_minPulsewidthInMs) +
                           (m_maxPulsewidthInMs - m_minPulsewidthInMs) * (n_degree / m_servoRangeInDegrees));
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
