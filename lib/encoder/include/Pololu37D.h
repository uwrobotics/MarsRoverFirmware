#pragma once

#include "Encoder.h"
#include "QEI.h"

namespace Encoder {
class Pololu37D final : public Encoder {
  // link to the Polulu37D encoder:
  // https://www.pololu.com/product/2821
 public:
  typedef struct Config {
    PinName ChannelA, ChannelB, Index;
    float offsetDeg;
  } Config;

  Pololu37D(const Config &config);
  Pololu37D(Pololu37D &)  = delete;
  Pololu37D(Pololu37D &&) = delete;
  ~Pololu37D()            = default;

  bool getAngleDeg(float &angle) override;
  bool getAngularVelocityDegPerSec(float &speed) override;
  bool reset() override;

 private:
  void read();
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
