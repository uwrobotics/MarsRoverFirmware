#pragma once

#include "Encoder.h"
#include "QEI.h"

namespace EncoderRelative_Quadrature {
class EncoderRelative_Quadrature : public Encoder {
  typedef struct Config {
    PinName qei;
    float degreesPerUnit, zeroOffset;
  } Config;

 public:
  EncoderRelative_Quadrature(const Config &config);
  ~EncoderRelative_Quadrature();

  bool getAngle_Degrees(float &theta) override;
  bool getVelocity_DegreesPerSec(float &thetaDot) override;

  bool reset() override;

 private:
  QEI m_QEI;
  float m_degreesPerUnit;
  float m_zeroOffsetDegrees;
};
}  // namespace EncoderRelative_Quadrature
