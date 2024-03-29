#pragma once

#include "Sensor.h"
#include "mbed.h"

namespace Sensor {
class PollingSensors final : Sensor {
 public:
  PollingSensors(PinName moisture_in, PinName co2_in);

  float read() override {}

  float alternateRead() override {}

  bool getStatus() const override {}

  [[nodiscard]] bool reset() override {}

  [[nodiscard]] bool update() override {}

  /* Functions specific to sensor modules*/

  /* This function returns Moisture sensor value
   * 0-300 - dry
   * 300-700 - humid
   * 700-900 - wet
   * Returns -1 for fault
   * */
  float moisture_monitoring();

  /* This function returns CO2 sensor value in ppm\
   * Returns -1 for fault and 0 for preheating
   * */
  float C02_monitoring();

 private:
  /*Pins configuration for moisture sensing*/
  AnalogIn m_moisture_in_adc;

  /*Pins configuration for CO2 sensing*/
  AnalogIn m_CO2_in_adc;
};
}  // namespace Sensor