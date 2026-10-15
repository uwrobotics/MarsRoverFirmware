#include "TutorialServo.h"

#include <algorithm>

// Servo expects one pulse every 20 ms (50 Hz)
constexpr int k_servoPeriodMs = 20;

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {
  m_servoPwmOut.period_ms(k_servoPeriodMs);
}

void TutorialServo::setPositionInDegrees(const float degrees) {
  // Keep the angle inside what the servo can physically reach
  const float clampedDegrees = std::clamp(degrees, 0.0f, m_servoRangeInDegrees);

  // Map 0..range degrees onto min..max pulse width
  const float pulseMs =
      m_minPulsewidthInMs + (clampedDegrees / m_servoRangeInDegrees) * (m_maxPulsewidthInMs - m_minPulsewidthInMs);

  // pulsewidth() takes seconds, not milliseconds
  m_servoPwmOut.pulsewidth(pulseMs / 1000.0f);
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