#include "EncoderAbsolute_PWM.h"

Encoder::Absolute_PWM::Absolute_PWM(PwmIn pwmIn, float degreesPerUnit, float zeroOffsetDegrees) : m_pwmIn(pwmIn), m_degreesPerUnit(degreesPerUnit), m_zeroOffsetDegrees(zeroOffsetDegrees) {}

Encoder::Absolute_PWM::Absolute_PWM(const Config &config)
    : Absolute_PWM(config.pwmIn, config.degreesPerUnit, config.zeroOffsetDegrees) {}

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
