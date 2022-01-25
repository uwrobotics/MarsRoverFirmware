#include "TutorialServo.hpp"

#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {
  m_servoPwmOut.period_ms(20);
}

void TutorialServo::setPositionInDegrees(const float degrees) {
  if (degrees > m_servoRangeInDegrees) {
    m_servoPwmOut.pulsewidth(m_maxPulsewidthInMs / 1000);
  } else if (degrees < 0) {
    m_servoPwmOut.pulsewidth(m_minPulsewidthInMs / 1000);
  } else {
    m_servoPwmOut.pulsewidth(m_minPulsewidthInMs / 1000 +
                             (degrees / (m_servoRangeInDegrees * 1000)) * (m_maxPulsewidthInMs - m_minPulsewidthInMs));
  }
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
