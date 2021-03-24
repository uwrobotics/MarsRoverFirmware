#include "TutorialServo.h"

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
  if (degrees <= m_servoRangeInDegrees && degrees >= 0) {
    m_servoPwmOut.pulsewidth_ms(m_minPulsewidthInMs +
                                (degrees / m_servoRangeInDegrees) * (m_maxPulsewidthInMs - m_minPulsewidthInMs));
  }
}

float TutorialServo::getServoRangeInDegrees() const {
  return m_servoRangeInDegrees;
}

float TutorialServo::getMinPulsewidthInMs() const {
  return m_minPulsewidthInMs;
}

float TutorialServo::getMaxPulsewidthInMs() const {
  return m_maxPulsewidthInMs;
}
