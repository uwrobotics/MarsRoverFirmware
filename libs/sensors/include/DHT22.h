#pragma once

#include "Sensor.h"
#include "mbed.h"

namespace Sensor {
class DHT22 final : public Sensor {
 public:
  DHT22(PinName data);

  // Returns internal humidity value (and temperature? Since all transmitted at the same time...)
  float read() override;

  // Returns internal temperature reading *Probably irrelevant?
  float alternateRead() override;

  // Returns whether sensor responds to request?
  bool getStatus() const override;

  // irrelevant probably
  [[nodiscard]] bool reset() override;

  // Requests relative humidity and temperature data from sensor
  [[nodiscard]] bool update() override;

 private:
  mutable DigitalInOut m_data;
  bool updateData();

  float m_humidity, m_temperature;
};
}  // namespace Sensor