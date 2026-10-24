#include "TutorialServo.h"

#include <algorithm>

namespace {
constexpr int SERVO_PERIOD_MS = 20;  // 50Hz
}

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {
  m_servoPwmOut.period_ms(SERVO_PERIOD_MS);
  setPositionInDegrees(0.0f);
}

void TutorialServo::setPositionInDegrees(const float degrees) {
  const float clampedDegrees = std::clamp(degrees, 0.0f, m_servoRangeInDegrees);
  const float pulsewidthInMs =
      m_minPulsewidthInMs + (clampedDegrees / m_servoRangeInDegrees) * (m_maxPulsewidthInMs - m_minPulsewidthInMs);
  m_servoPwmOut.pulsewidth_us(static_cast<int>(pulsewidthInMs * 1000.0f));
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
