#include "TutorialServo.h"

#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {
  m_servoPwmOut.period_ms(20);
}  // setting the servo period to 20 ms

void TutorialServo::setPositionInDegrees(const float degrees) {
  if (degrees > m_servoRangeInDegrees) {
    m_servoPwmOut.pulsewidth(m_maxPulsewidthInMs);
  } else {
    float pulse = (1 + degrees / m_servoRangeInDegrees) / 1000;

    if (pulse < m_minPulsewidthInMs) {
      m_servoPwmOut.pulsewidth(m_minPulsewidthInMs);
    } else {
      m_servoPwmOut.pulsewidth(pulse);
    }
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