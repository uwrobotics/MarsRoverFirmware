#pragma once

#include "Sensor.h"

namespace Sensor {
class CurrentSensor final : Sensor {
 public:
  CurrentSensor() = default;
  bool read(float &sensorReading) override;
  bool alternateRead(float &sensorReading) override;
  bool getStatus() const override;
  bool reset() override;
};
}  // namespace Sensor
