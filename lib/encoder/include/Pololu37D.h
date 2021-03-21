#pragma once

#include "Encoder.h"
#include "QEI.h"

// younes todo update pololu structure once yehia merges in his stuff maybe

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

  // see todo at the top
  [[nodiscard]] bool update() override {
    return false;
  }
  float getAngleDeg() override {
    return 0;
  }
  float getAngularVelocityDegPerSec() override {
    return 0;
  }

  bool getAngleDeg(float &angle);
  bool getAngularVelocityDegPerSec(float &speed);
  [[nodiscard]] bool reset() override;

 private:
  GPIO::QEI m_QEI;

  Mutex m_mutex;

  static constexpr float m_degreesPerCount = 360.0 / 64.0;  // since encoder is 64 CPR
  float m_zeroOffsetDeg;
};
}  // namespace Encoder
