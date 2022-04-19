#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs) m_maxPulsewidthInMs(maxPulsewidthInMs) {
  // period depends on the servo itself, check datasheet
  servoPwnOut.period_ms(20);
}

void TutorialServo::setPositionInDegrees(const float degrees) {
  // if user set degrees to greater than servo range, set servo to max position
  if (degree <= 0) {
    m_servoPwmOut.pulsewidth(m_minPulsewidthInMs);
    return;
  }
  // if user set degrees to greater than servo range, set servo to max position
  if (degrees >= m_servoRangeInDegrees) {
    m_servoPwmOut.pulsewidth(m_maxPulsewidthInMs);
    return;
  };

  // ratio of degrees/range = pulseWidth/(maxPulseWidth - minPulseWidth)
  // map the desire degree to its corresponding pulseWidth according to the ratio above
  m_servoPwmOut.pulsewidth(
      (m_minPulsewidthInMs + (degrees / m_servoRangeInDegrees) * (m_maxPulsewidthInMs - m_minPulsewidthInMs)) / 1000);
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