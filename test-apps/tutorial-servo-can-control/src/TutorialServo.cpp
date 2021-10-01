#include "TutorialServo.h"

#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees,
                             float minPulsewidthInMs, float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin), m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {
  m_servoPwmOut.period_ms(20);
}

void TutorialServo::setPositionInDegrees(const float degrees) {
  float pulsewidth;

  if (degrees > m_servoRangeInDegrees) {
    pulsewidth = m_maxPulsewidthInMs;
  } else if (degrees < 0) {
    pulsewidth = m_minPulsewidthInMs;
  } else {
    pulsewidth =
        m_minPulsewidthInMs + (degrees / m_servoRangeInDegrees) *
                                  (m_maxPulsewidthInMs - m_maxPulsewidthInMs);
  }

  m_servoPwmOut.pulsewidth_ms(pulsewidth);
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