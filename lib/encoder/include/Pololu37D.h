#pragma once

#include "Encoder.h"
#include "QEI.h"

namespace Encoder {
class Pololu37D final : public Encoder {
  // link to the encoder:
  // https://www.pololu.com/product/2821
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

  bool getAngleDeg(float &angle) override;
  bool getAngularVelocityDegPerSec(float &speed) override;
  bool reset() override;

 private:
  bool read();
  float return_delta_time();

  // tracks the previous angle in degrees reading to calculate speed
  float m_previous_angle_deg;
  // holds the current angle in degrees
  float m_current_angle_deg;
  // holds the measured speed in degrees per second
  float m_speed_deg_per_second;
  // holds the time since last measurement
  float m_delta_time_ns;

  GPIO::QEI m_QEI;
  Timer m_timer;
  Mutex m_mutex;

  static constexpr float m_degreesPerCount = 360.0 / 64.0;  // since encoder is 64 CPR
  float m_zeroOffsetDeg;
};
}  // namespace Encoder
