#pragma once

#include "Encoder.h"
#include "QEI.h"

namespace Encoder {
class Relative_Quadrature : public Encoder {
  typedef struct Config {
    PinName qei;
    float degreesPerUnit, zeroOffset;
  } Config;

 public:
  Relative_Quadrature(const Config &config);
  ~Relative_Quadrature();

  bool getAngle_Degrees(float &theta) override;
  bool getVelocity_DegreesPerSec(float &thetaDot) override;

  bool reset() override;

 private:
  QEI m_QEI;
  float m_degreesPerUnit;
  float m_zeroOffsetDegrees;
};
}  // namespace EncoderRelative_Quadrature
