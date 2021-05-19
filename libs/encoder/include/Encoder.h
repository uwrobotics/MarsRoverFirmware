#pragma once

namespace Encoder {
class Encoder {
 public:
  virtual ~Encoder(){};

  [[nodiscard]] virtual bool update() = 0;
  [[nodiscard]] virtual bool reset()  = 0;

  virtual float getAngleDeg()                 = 0;
  virtual float getAngularVelocityDegPerSec() = 0;
};
}  // namespace Encoder
