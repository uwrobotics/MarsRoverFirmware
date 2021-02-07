#pragma once

namespace Encoder {
class Encoder {
 public:
  virtual ~Encoder(){};

  virtual bool getAngleDeg(float &angle)                 = 0;
  virtual bool getAngularVelocityDegPerSec(float &speed) = 0;
  virtual bool reset()                                   = 0;
};
}  // namespace Encoder
