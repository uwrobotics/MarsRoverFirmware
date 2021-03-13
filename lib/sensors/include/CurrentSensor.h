#pragma once

#include "Sensor.h"

namespace Sensor {
// TODO: rename current sensor class to technical name of sensor (ie. AdafruitSTEMMA)
class CurrentSensor final : Sensor {
 public:
  CurrentSensor(PinName clk, PinName miso, PinName cs = NC);
  bool read(float &sensorReading) override;
  bool alternateRead(float &sensorReading) override;
  bool getStatus() const override;
  bool reset() override;
};
}  // namespace Sensor
