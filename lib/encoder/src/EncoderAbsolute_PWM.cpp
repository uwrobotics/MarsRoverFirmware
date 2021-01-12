#include "EncoderAbsolute_PWM.h"

EncoderAbsolute_PWM::EncoderAbsolute_PWM(t_encoderConfig encoderConfig)
    : Encoder(encoderConfig),
      m_pwmIn(encoderConfig.pin_PWM),
      m_degreesPerUnit(encoderConfig.degreesPerUnit),
      m_zeroOffsetDegrees(encoderConfig.zeroOffset_Degrees) {}

EncoderAbsolute_PWM::~EncoderAbsolute_PWM() {}

float EncoderAbsolute_PWM::getAngle_Degrees() {
  return (m_pwmIn.avgDutyCycle() * m_degreesPerUnit) - m_zeroOffsetDegrees;
}

float EncoderAbsolute_PWM::getVelocity_DegreesPerSec() {
  return m_pwmIn.avgDutyCycleVelocity() * m_degreesPerUnit;
}

mbed_error_status_t EncoderAbsolute_PWM::reset() {
  m_zeroOffsetDegrees = m_pwmIn.avgDutyCycle() * m_degreesPerUnit;
  return MBED_SUCCESS;
}
