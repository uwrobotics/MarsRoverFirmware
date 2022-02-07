#pragma once

#include "Sensor.h"
#include "mbed.h"

class PollingSensors final : Sensor {
 public:
  PollingSensors(PinName clk, PinName miso, PinName cs = NC);
  float read() override;
  float alternateRead() override;
  bool getStatus() const override;
  [[nodiscard]] bool reset() override;
  [[nodiscard]] bool update() override;
  
  //Add functions specific to sensor modules  
};
