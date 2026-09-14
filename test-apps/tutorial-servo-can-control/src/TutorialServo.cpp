#include "TutorialServo.h"

#include <algorithm>
#include <cmath>

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {
  MBED_ASSERT(std::isfinite(servoRangeInDegrees) && servoRangeInDegrees > 0.0f);
  MBED_ASSERT(std::isfinite(minPulsewidthInMs) && minPulsewidthInMs > 0.0f);
  MBED_ASSERT(std::isfinite(maxPulsewidthInMs) && maxPulsewidthInMs > minPulsewidthInMs && maxPulsewidthInMs < 20.0f);
  m_servoPwmOut.period_ms(20);
  setPositionInDegrees(0.0f);
}

void TutorialServo::setPositionInDegrees(const float degrees) {
  if (!std::isfinite(degrees)) {
    return;
  }

  const float position       = std::clamp(degrees, 0.0f, m_servoRangeInDegrees) / m_servoRangeInDegrees;
  const float pulsewidthInMs = m_minPulsewidthInMs + position * (m_maxPulsewidthInMs - m_minPulsewidthInMs);
  m_servoPwmOut.pulsewidth(pulsewidthInMs / 1000.0f);
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
