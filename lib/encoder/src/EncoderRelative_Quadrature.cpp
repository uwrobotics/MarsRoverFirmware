#include "EncoderRelative_Quadrature.h"

Encoder::Relative_Quadrature(t_encoderConfig encoderConfig)
    : Encoder(encoderConfig),
      m_QEI(encoderConfig.pin_ChannelA, encoderConfig.pin_ChannelB, encoderConfig.pin_Index,
            encoderConfig.quadratureEncodingType),
      m_degreesPerUnit(encoderConfig.degreesPerUnit),
      m_zeroOffsetDegrees(encoderConfig.zeroOffset_Degrees) {}

Encoder::~Relative_Quadrature() {}

Encoder::t_encoderType Encoder::Relative_Quadrature::getType() {
  return encoderType::relative;
}

float Encoder::Relative_Quadrature::getAngle_Degrees() {
  return (m_QEI.getPulses() * m_degreesPerUnit) - m_zeroOffsetDegrees;
}

float Encoder::Relative_Quadrature::getVelocity_DegreesPerSec() {
  return m_QEI.getPulseVelocity_PulsesPerSec() * m_degreesPerUnit;
}

mbed_error_status_t Encoder::Relative_Quadrature::reset() {
  m_QEI.reset();
  return MBED_SUCCESS;
}
