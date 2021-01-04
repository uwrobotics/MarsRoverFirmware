#pragma once

#include "mbed.h"
#include "Sensor.h"

class MoistureSensor : public Sensor {
 public:
  MoistureSensor(PinName sda, PinName scl);
  ~MoistureSensor();

  //functions to read sensor data
  //primary read -> reads moisture data
  //returns true on success, requires parameter to be passed in to store
  bool read(float &sensorReading) override;
  //alternate read -> reads temperature data
  //returns true on success, requires parameter to be passed in to store
  bool alternateRead(float &sensorReading) override;

  void reset() override;
  //reads the HW ID and checks that it is correct
  bool getStatus() override;

 private:
  I2C i2c_;
};
