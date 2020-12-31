#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1,
                             float maxPulsewidthInMs = 2) {
  m_servoPwmOut         = servoPwmOut(sevrvoPin);
  m_servoRangeInDegrees = servoRangeInDegrees;
  m_minPulsewidthInMs   = minPulsewidthInMs;
  m_maxPulsewidthInMs   = maxPulsewidthInMs;
}

void TutorialServo::setPositionInDegrees(const float degrees) {
  servoPwmOut.pulsewidth(.001 + .001 * degrees / servoRangeInDegrees);
}

float getServoRangeInDegrees( ) const{
  return m_servoRangeInDegrees;
}
float getMinPulseWidthInMs( ) const{
  return m_minPulsewidthInMs;
}
float getMaxPulseWidthInMs( ) const{
  return m_MaxPulsewidthInMs;
}
