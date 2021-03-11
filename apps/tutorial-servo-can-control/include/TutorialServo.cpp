#include "TutorialServo.h"
#include "mbed.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs, float maxPulsewidthInMs)
  : m_servoPin(servoPin), m_servoRangeInDegrees(servoRangeInDegrees), m_minPulsewidthInMs(minPulsewidthInMs), m_maxPulsewidthInMs(maxPulsewidthInMs)
{
  PwmOut servoPwmOut(servoPin);
  servoPwmOut.period_ms(20);
}

void setPositionInDegrees(const float degrees)
{
  if (degrees <= m_servoRangeInDegrees && degrees >= 0)
  {
    servoPwmOut.pulsewidth_ms(m_minPulsewidthInMs + degrees/m_servoRangeInDegrees)
  }
}

float getServoRangeInDegrees() const
{
  return m_servoRangeInDegrees;
}

float getMinPulsewidthInMs() const
{
  return m_minPulsewidthInMs;
}

float getMaxPulsewidthInMs() const
{
  return m_maxPulsewidthInMs;
}
