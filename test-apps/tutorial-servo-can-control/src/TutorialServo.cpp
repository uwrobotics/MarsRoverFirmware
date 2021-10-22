#include "TutorialServo.h"

#include <cstdlib>

#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {
  m_servoPwmOut.period_ms(20);
}

// Set servo position (ex. 45 deg)
void setPositionInDegrees(const float degrees) {
  if (degrees >= 0 && degrees <= m_servoRangeInDegrees) {
    m_servoPwmOut.pulsewidth(
        (m_minPulsewidthInMs + (degrees * (abs(m_maxPulsewidthInMs - m_minPulsewidthInMs) / m_servoRangeInDegrees))) /
        1000);
  } else if (degrees < 0) {
    m_servoPwmOut.pulsewidth(m_minPulsewidthInMs);
  } else {
    m_servoPwmOut.pulsewidth(m_maxPulsewidthInMs);
  }
}

// Get the servo range in degrees (ex: 90 deg)
float getServoRangeInDegrees() const {
  return m_servoRangeInDegrees;
}

// Get the min pulse width in ms (ex: 1ms)
float getMinPulseWidthInMs() const {
  return m_minPulsewidthInMs;
}

// Get the max pulse width in ms (ex: 2ms)
float getMaxPulseWidthInMs() const {
  return m_maxPulsewidthInMs;
}