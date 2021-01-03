#pragma once

#include "mbed.h"

namespace URM04Sensor {

class URM04Sensor {

 protected:
  // constants
  static constexpr int BAUD_RATE     = 19200;
  static constexpr int START_ADDRESS = 0x11;
  static constexpr int LOW           = 0;

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
  //serial
  BufferedSerial serial; 
  //variable to check if success_read
  bool success;
  //sensor distance
  float m_distance; 

 public:
  // constructor
  URM04Sensor(PinName trig_pin, PinName _RX, PinName _TX);

  //Trigger Sensor
  void trigger_sensor();

  //Read Distance
  float read_distance();
};

}  // namespace sensor