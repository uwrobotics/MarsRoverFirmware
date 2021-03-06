#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {}

// Ensure degree entered is within the range specified in m_servoRangeInDegrees
float TutorialServo::fixValue(float value, float minValue, float maxValue) {
  if (value < minValue) {
    return minValue;
  } else if (value > maxValue) {
    return maxValue;
  } else
    return value;
}

void TutorialServo::setPositionInDegrees(const float degrees) {
  // Set the pulse width on the PWM Pin using the degrees entered and
  //    ensure pulsewidth is within the max and min Pulse width values
  // figure out the function for the degrees/ pulse width line to use for the pulsewidth function
  // float pWidth = m_servoPwmOut.pulsewidth(m_minPulsewidthInMs + (fixValue(degrees,
  //     0, m_servoRangeInDegrees)/m_servoRangeInDegrees));
  m_servoPwmOut.pulsewidth_ms(((m_maxPulsewidthInMs - m_minPulsewidthInMs) / m_servoRangeInDegrees) *
                                  fixValue(degrees, 0, m_servoRangeInDegrees) +
                              m_minPulsewidthInMs);
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