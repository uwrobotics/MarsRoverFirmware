#pragma once

#include "Sensor.h"

namespace MoistureSensor {
typedef struct {
  PinName sda, scl;
} Config;

class MoistureSensor : public Sensor::Sensor {
 public:
  MoistureSensor(PinName sda, PinName scl);

  MoistureSensor(const Config &config);
  ~MoistureSensor();

  /*functions to read sensor data
    primary read -> reads moisture data
    returns true on success, requires parameter to be passed in to store*/
  bool read(float &sensorReading) override;
  /*alternate read -> reads temperature data
    returns true on success, requires parameter to be passed in to store*/
  bool alternateRead(float &sensorReading) override;

  bool reset() override;
  // reads the HW ID and checks that it is correct
  bool getStatus() override;

 private:
  I2C m_i2c;
};
}  // namespace MoistureSensor
