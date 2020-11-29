#pragma once

#include "mbed.h"
#include "Sensor.h"

class MoistureSensor : public Sensor {
 public:
  MoistureSensor(PinName sda, PinName scl) override;
  ~MoistureSensor();

  //functions to read sensor data
  //primary read -> reads moisture data
  float read() override;
  //alternate read -> reads temperature data
  float alternateRead() override;

  void reset() override;
  //reads the HW ID and checks that it is correct
  bool getStatus() override;

 private:
  I2C i2c_;
};
