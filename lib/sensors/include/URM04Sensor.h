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

 private:
  // trigger pin
  DigitalOut m_trigPin;
  // start address
  uint8_t startAddr;
  // command buffer
  uint8_t cmdst[10];
  // UART protocol pins
  PinName RX;
  PinName TX;
  // serial
  BufferedSerial serial;
  // Trigger Sensor
  void trigger_sensor(float& distance);

 public:
  // constructor
  // allows for default address of device to be changed within constructor
  // @param default_address is a fixed parameter that is already set to the default address
  URM04Sensor(PinName trig_pin, PinName _RX, PinName _TX, uint8_t default_address = START_ADDRESS);

  // Read Distance in CENTIMETER returns true if successful read
  // pass by reference a variable to hold the distance
  bool read_distance(float& distance);

  // return true if address has been changed successfully
  // takes in a address as as paramater
  bool set_address(uint8_t _address);
};

}  // namespace URM04Sensor