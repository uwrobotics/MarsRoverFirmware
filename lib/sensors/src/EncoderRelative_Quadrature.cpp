#include "EncoderRelative_Quadrature.h"

EncoderRelative_Quadrature::EncoderRelative_Quadrature(t_encoderConfig encoderConfig)
    : Encoder(encoderConfig),
      m_QEI(encoderConfig.pin_ChannelA, encoderConfig.pin_ChannelB, encoderConfig.pin_Index,
            encoderConfig.quadratureEncodingType),
      m_degreesPerUnit(encoderConfig.degreesPerUnit),
      m_zeroOffset_Degrees(encoderConfig.zeroOffset_Degrees) {}

EncoderRelative_Quadrature::~EncoderRelative_Quadrature() {}

Encoder::t_encoderType EncoderRelative_Quadrature::getType() {
  return encoderType::relative;
}

float EncoderRelative_Quadrature::getAngle_Degrees() {
  return (m_QEI.getPulses() * m_degreesPerUnit) - m_zeroOffset_Degrees;
}

float EncoderRelative_Quadrature::getVelocity_DegreesPerSec() {
  return m_QEI.getPulseVelocity_PulsesPerSec() * m_degreesPerUnit;
}

mbed_error_status_t EncoderRelative_Quadrature::reset() {
  m_QEI.reset();
  return MBED_SUCCESS;
}
