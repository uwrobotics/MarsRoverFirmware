#pragma once

#include "Sensor.h"

namespace Sensor {
class AdafruitSTEMMA final : public Sensor {
 public:
  typedef struct Config {
    PinName sda, scl;
  } Config;
  
  AdafruitSTEMMA(PinName sda, PinName scl);
  AdafruitSTEMMA(const Config &config);
  AdafruitSTEMMA(AdafruitSTEMMA &)  = delete;
  AdafruitSTEMMA(AdafruitSTEMMA &&) = delete;
  ~AdafruitSTEMMA()                 = default;

  /*functions to read sensor data
    primary read -> reads moisture data
    returns true on success, requires parameter to be passed in to store*/
  bool read(float &sensorReading) override;

  /*alternate read -> reads temperature data
    returns true on success, requires parameter to be passed in to store*/
  bool alternateRead(float &sensorReading) override;

  bool reset() override;

  // reads the HW ID and checks that it is correct
  bool getStatus() const override;

 private:
  mutable I2C m_i2c;

  static constexpr int Sensor_I2C_Address =
      0x36 << 1;  // MBED I2C uses 8 bit addressing, so addresses are left shifted by 1 (may need to be shifted by 2)

  static constexpr int Sensor_Status_Base   = 0x00;  // Base address registers for different modules
  static constexpr int Sensor_Moisture_Base = 0x0F;

  static constexpr int Sensor_Status_HW_ID = 0x01;  // Function address register for the sensor's HW ID
  static constexpr int Sensor_HW_ID_Code   = 0x55;  // Expected value for sensor HW ID

  static constexpr int Sensor_Moisture_Function = 0x10;  // Function address registers for various modules
  static constexpr int Sensor_Temp_Function     = 0x04;
  static constexpr int Sensor_Status_Reset      = 0x7F;
};
}  // namespace Sensor
