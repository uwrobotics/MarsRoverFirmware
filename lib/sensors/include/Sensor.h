#pragma once

namespace Sensor {
class Sensor {
 public:
  virtual float read()                = 0;
  virtual float alternateRead()       = 0;
  virtual bool getStatus() const      = 0;
  [[nodiscard]] virtual bool reset()  = 0;
  [[nodiscard]] virtual bool update() = 0;
  virtual ~Sensor(){};
};
}  // namespace Sensor
