#pragma once

#include "mbed.h"

namespace URM04Sensor {

class URM04Sensor {
 protected:
  // constants
  static constexpr int BAUD_RATE     = 19200;
  static constexpr int START_ADDRESS = 0x11;
  static constexpr int LOW           = 0;
  static constexpr int HIGH          = 1;
  static constexpr float MAX_FLOAT   = std::numeric_limits<float>::max();

 private:
  // trigger pin
  DigitalOut m_trigPin;
  // start address
  uint8_t m_startAddr;
  // command buffer
  uint8_t m_cmdst[8];
  // serial
  BufferedSerial m_serial;
  // Trigger Sensor
  bool trigger_sensor();

 public:
  // constructor
  // allows for default address of device to be changed within constructor
  // @param default_address is a fixed parameter that is already set to the default address
  URM04Sensor(PinName trig_pin, PinName _RX, PinName _TX, uint8_t default_address = START_ADDRESS);

  // Read Distance in CENTIMETER returns true if successful read
  // pass by reference a variable to hold the distance
  // sensors range is 4cm - 500cm
  // will give MAX_FLOAT value if out of range
  bool read_distance(float& distance);

  // return true if address has been changed successfully
  // takes in the new address to set to as paramater
  bool set_address(uint8_t _address);
};

}  // namespace URM04Sensor
