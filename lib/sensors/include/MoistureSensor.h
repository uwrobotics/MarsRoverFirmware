#pragma once

#include "mbed.h"
#include "Sensor.h"

class MoistureSensor : public Sensor {
 public:
  MoistureSensor(PinName sda, PinName scl);
  ~MoistureSensor() = default;

  //functions to read sensor data
  //primary read -> reads moisture data
  float primaryRead();
  //alternate read -> reads temperature data
  float alternateRead();

  void resetSensor();
  //reads the HW ID and checks that it is correct
  bool getSensorStatus();

 private:
  I2C i2c_;
};
