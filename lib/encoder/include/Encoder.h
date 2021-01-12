#pragma once

namespace Encoder {
class Encoder {
 public:
  virtual ~Encoder(){};
  virtual bool getAngle_Degrees(float &theta)             = 0;
  virtual bool getVelocity_DegreesPerSec(float &thetaDot) = 0;
  virtual bool reset()                                    = 0;
};
}  // namespace Encoder