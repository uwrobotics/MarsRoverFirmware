#include "EncoderRelative_Quadrature.h"

Encoder::Relative_Quadrature::Relative_Quadrature(const Config &config)
    : m_QEI(config.pin_ChannelA, config.pin_ChannelB, config.pin_Index, config.quadratureEncodingType),
      m_degreesPerUnit(config.degreesPerUnit),
      m_zeroOffsetDegrees(config.zeroOffsetDegrees) {}

Encoder::Relative_Quadrature::~Relative_Quadrature() {}

bool Encoder::Relative_Quadrature::getAngle_Degrees(float &theta) {
  theta = (m_QEI.getPulses() * m_degreesPerUnit) - m_zeroOffsetDegrees;
  return true;
}

bool Encoder::Relative_Quadrature::getVelocity_DegreesPerSec(float &thetaDot) {
  thetaDot = m_QEI.getPulseVelocity_PulsesPerSec() * m_degreesPerUnit;
  return true;
}

bool Encoder::Relative_Quadrature::reset() {
  m_QEI.reset();
  return true;
}
