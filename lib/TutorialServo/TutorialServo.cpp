
#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {
  m_servoPwmOut.period(0.020);
}

void TutorialServo::setAngleRangeInDegrees(float degrees) {
  m_servoRangeInDegrees = degrees;
}

void TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs) {
  m_minPulsewidthInMs = minPulsewidthMs;
  m_maxPulsewidthInMs = maxPulsewidthMs;
}

void TutorialServo::setPositionInDegrees(float degrees) {
  if (degrees >= m_servoRangeInDegrees) {
    m_servoPwmOut.pulsewidth_us(m_maxPulsewidthInMs * 1000);
  } else if (degrees <= 0) {
    m_servoPwmOut.pulsewidth_us(m_minPulsewidthInMs * 1000);
  } else {
    m_servoPwmOut.pulsewidth_us(int(
        ((m_maxPulsewidthInMs - m_minPulsewidthInMs) * degrees / m_servoRangeInDegrees + m_minPulsewidthInMs) * 1000));
  }
}

float TutorialServo::getAngleRangeInDegrees() {
  return m_servoRangeInDegrees;
}
