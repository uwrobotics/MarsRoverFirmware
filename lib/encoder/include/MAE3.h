#pragma once

#include "Encoder.h"
#include "PwmIn.h"

namespace Encoder {
class MAE3 final : public Encoder {
 public:
  typedef struct Config {
    PinName pwm;
    float offsetDeg;
  } Config;

  MAE3(const Config &config);
  MAE3(MAE3 &)  = delete;
  MAE3(MAE3 &&) = delete;
  ~MAE3()       = default;

  bool getAngleDeg(float &angle) override;
  bool getAngularVelocityDegPerSec(float &speed) override;
  bool reset() override;

 private:
  GPIO::PwmIn m_pwmIn;

  Mutex m_mutex;

  static constexpr float m_degreesPerUnit = 360;
  float m_zeroOffsetDeg;
};
}  // namespace Encoder
