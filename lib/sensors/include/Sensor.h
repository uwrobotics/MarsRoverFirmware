#pragma once

namespace Sensor {
class Sensor {
 public:
  virtual bool read(float &sensorReading)          = 0;
  virtual bool alternateRead(float &sensorReading) = 0;
  virtual bool getStatus() const                   = 0;
  virtual bool reset()                             = 0;
  virtual ~Sensor(){};
};
}  // namespace Sensor
