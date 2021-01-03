#pragma once

#include "mbed.h"

namespace sensor {

class URM04Sensor {
 protected:
  // constants
  static constexpr int urmAccount    = 1;
  static constexpr int MAX_TRIES     = 10;
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
  // state machine reading step
  uint8_t readingStep;
  // arrays needed to read and write distance from sensor
  int urmID[urmAccount];
  uint32_t urmData[urmAccount];
  // timer related members
  uint64_t managerTimer;
  Timer clock;
  // UART protocol pins
  PinName RX;
  PinName TX;
  // successful read
  bool read_success;

  // trigger the mesausrements from URM04
  void urmTrigger(int id);
  // reads the distance from URM04
  void urmReader(int id);
  // transmit commands
  void transmitCommands();

  // analyzes the distance**
  void analyzeUrmData(uint8_t cmd[]);
  // runs the sensor - starts giving commands to the sensor
  void runUrm04();
  // decodes URM04 data
  void decodeURM04();

 public:
  // constructor
  URM04Sensor(PinName trig_pin, PinName _RX, PinName _TX);
  // destructor
  ~URM04Sensor();
  bool read();
  void print_distance();
};

}  // namespace sensor