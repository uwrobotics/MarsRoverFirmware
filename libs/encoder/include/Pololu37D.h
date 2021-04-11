#pragma once

#include "Encoder.h"
#include "QEI.h"

namespace Encoder {
class Pololu37D final : public Encoder {
 public:
  typedef struct Config {
    PinName ChannelA, ChannelB, Index;
    float offsetDeg;
    GPIO::QEI::Encoding quadratureEncoding;
  } Config;

  Pololu37D(const Config &config);
  Pololu37D(Pololu37D &)  = delete;
  Pololu37D(Pololu37D &&) = delete;
  ~Pololu37D()            = default;

  float getAngleDeg() override;
  float getAngularVelocityDegPerSec() override;
  [[nodiscard]] bool reset() override;
  [[nodiscard]] bool update() override;

 private:
  GPIO::QEI m_QEI;

  Mutex m_mutex;

  float m_angle = 0, m_speed = 0;

  static constexpr float m_degreesPerCount = 360.0 / 64.0;  // since encoder is 64 CPR
  float m_zeroOffsetDeg;
};
}  // namespace Encoder
