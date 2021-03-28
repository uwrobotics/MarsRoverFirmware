#pragma once

#include "Sensor.h"

namespace Sensor {
// TODO: rename current sensor class to technical name of sensor (ie. AdafruitSTEMMA)
class CurrentSensor final : Sensor {
 public:
  CurrentSensor(PinName clk, PinName miso, PinName cs = NC);
  float read() override;
  float alternateRead() override;
  bool getStatus() const override;
  [[nodiscard]] bool reset() override;
  [[nodiscard]] bool update() override;
};
}  // namespace Sensor
