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
    m_servoPwmOut.pulsewidth_ms(m_maxPulsewidthInMs);
  } else if (degrees >= 0 && degrees <= m_servoRangeInDegrees) {
    // find the percentage of the pulse ms range to be moved, multuply it by the pulse ms range, then add it to your min
    // pulse ms offset
    m_servoPwmOut.pulsewidth_ms((degrees / m_servoRangeInDegrees) * (m_maxPulsewidthInMs - m_minPulsewidthInMs) +
                                m_minPulsewidthInMs);
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