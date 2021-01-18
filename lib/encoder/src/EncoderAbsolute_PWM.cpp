#include "EncoderAbsolute_PWM.h"

Encoder::Absolute_PWM::Absolute_PWM(const Config &config)
    : m_pwmIn(config.pwmIn), m_degreesPerUnit(config.degreesPerUnit), m_zeroOffsetDegrees(config.zeroOffsetDegrees) {}

Encoder::Absolute_PWM::~Absolute_PWM() {}

bool Encoder::Absolute_PWM::getAngle_Degrees(float &theta) {
  theta = (m_pwmIn.avgDutyCycle() * m_degreesPerUnit) - m_zeroOffsetDegrees;
  return true;
}

bool Encoder::Absolute_PWM::getVelocity_DegreesPerSec(float &thetaDot) {
  thetaDot = m_pwmIn.avgDutyCycleVelocity() * m_degreesPerUnit;
  return true;
}

bool Encoder::Absolute_PWM::reset() {
  m_zeroOffsetDegrees = m_pwmIn.avgDutyCycle() * m_degreesPerUnit;
  return true;
}
