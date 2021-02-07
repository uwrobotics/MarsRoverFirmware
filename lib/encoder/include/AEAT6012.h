#pragma once

#include "Encoder.h"
#include <mutex>

namespace Encoder {
class AEAT6012 : public Encoder {
 public:
  typedef struct Config {
    PinName pin;
    float offsetDeg;
  } Config;
  AEAT6012(const Config &config);
  bool getAngleDeg(float &angle) override;
  bool getAngularVelocityDegPerSec(float &speed) override;
  bool reset() override;

 private:
  PinName m_pin;

  Mutex m_mutex;

  float m_offsetDeg;
};
}  // namespace Encoder