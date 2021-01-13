#include "EncoderAbsolute_PWM.h"

Encoder::Absolute_PWM::Absolute_PWM(const Config &config)
    : Encoder(encoderConfig),
      m_pwmIn(encoderConfig.pin_PWM),
      m_degreesPerUnit(encoderConfig.degreesPerUnit),
      m_zeroOffsetDegrees(encoderConfig.zeroOffset_Degrees) {}

Encoder::Absolute_PWM::~Absolute_PWM() {}

bool Encoder::Absolute_PWM::getAngle_Degrees(float &theta) {
  theta = (m_pwmIn.avgDutyCycle() * m_degreesPerUnit) - m_zeroOffsetDegrees;
  return true;
}

bool Encoder::Absolute_PWM::getVelocity_DegreesPerSec(float &thetaDot) {
  thetaDot = m_pwmIn.avgDutyCycleVelocity() * m_degreesPerUnit;
  return true;
}

mbed_error_status_t Encoder::Absolute_PWM::reset() {
  m_zeroOffsetDegrees = m_pwmIn.avgDutyCycle() * m_degreesPerUnit;
  return MBED_SUCCESS;
}
