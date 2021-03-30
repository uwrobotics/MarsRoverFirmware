
#include "TutorialServo.h"

#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1,
                             float maxPulsewidthInMs = 2) {
  m_servoPwmOut = servoPin;
  m_servoPwmOut.period_ms(20);
  m_servoRangeInDegrees = servoRangeInDegrees;
  m_minPulsewidthInMs   = minPulsewidthInMs;
  m_maxPulsewidthInMs   = maxPulsewidthInMs;
}

void TutorialServo::setPositionInDegrees(const float degrees) {
  float pulse = m_minPulsewidthInMs + (degrees / m_servoRangeInDegrees);

  if (pulse > m_maxPulsewidthInMs)
    pulse = m_maxPulsewidthInMs;
  else if (pulse < m_minPulsewidthInMs)
    pulse = m_minPulsewidthInMs;

  m_servoPwmOut.pulsewidth_ms(pulse);
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
}
;