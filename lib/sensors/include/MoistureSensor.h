#pragma once

#include "mbed.h"
#include "Sensor.h"

class MoistureSensor : public Sensor {
 public:
  MoistureSensor(PinName sda, PinName scl);
  ~MoistureSensor() = default;

  //uint8_t Read_HW_ID();
  //void Reset_Sensor();
  //bool Is_Initialized();

  float primaryRead();
  float alternateRead();

 private:
  I2C i2c_;
};
