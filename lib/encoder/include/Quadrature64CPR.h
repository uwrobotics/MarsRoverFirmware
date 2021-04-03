#pragma once

#include "Encoder.h"
#include "QEI.h"

namespace Encoder {
class Quadrature64CPR final : public Encoder {
  // link to the Quadrature64CPR encoder:
  // https://www.pololu.com/product/2821
 public:
  typedef struct Config {
    PinName ChannelA, ChannelB, Index;
    float offsetDeg;
  } Config;

  Quadrature64CPR(const Config &config);
  Quadrature64CPR(Quadrature64CPR &)  = delete;
  Quadrature64CPR(Quadrature64CPR &&) = delete;
  ~Quadrature64CPR()                  = default;

  float getAngleDeg() override;
  float getAngularVelocityDegPerSec() override;
  bool reset() override;

 private:
  bool update() override;
  // tracks the previous angle in degrees reading to calculate speed
  float m_previous_angle_deg;
  // holds the current angle in degrees
  float m_current_angle_deg;
  // holds the measured speed in degrees per second
  float m_anglular_velocity_deg_per_sec;

  GPIO::QEI m_QEI;
  Timer m_timer;
  Mutex m_mutex;

  static constexpr float m_degreesPerCount = 360.0 / 64.0;  // since encoder is 64 CPR
  float m_zeroOffsetDeg;
};
}  // namespace Encoder
