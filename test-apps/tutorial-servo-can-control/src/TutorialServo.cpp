#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_pwm(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {
  m_pwm.period_ms(20);
}

void TutorialServo::setPositionInDegrees(const float degrees) {
  m_pwm.pulsewidth(
      ((degrees / m_servoRangeInDegrees) * (m_maxPulsewidthInMs - m_minPulsewidthInMs) + m_minPulsewidthInMs) / 1000);
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
