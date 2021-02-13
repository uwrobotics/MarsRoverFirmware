#pragma once

#include <mutex>

#include "Encoder.h"

/* TODO: create driver for encoder */
namespace Encoder {
class AEAT6012 : public Encoder {
 public:
  typedef struct Config {
    PinName clk, miso, CS;
    float offsetDeg;
  } Config;
  AEAT6012(const Config &config);
  bool getAngleDeg(float &angle) override;
  bool getAngularVelocityDegPerSec(float &speed) override;
  bool reset() override;

 private:
};
}  // namespace Encoder