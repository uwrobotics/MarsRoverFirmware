#pragma once

#include "Encoder.h"
#include "QEI.h"

namespace Encoder {
typedef struct {
  PinName qei;
  float degreesPerUnit, zeroOffsetDegrees;
  PinName pin_ChannelA, pin_ChannelB, pin_Index;
  t_quadratureEncodingType quadratureEncodingType;
} Config;

class Relative_Quadrature : public Encoder {
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
}  // namespace Encoder
