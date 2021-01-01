#pragma once
// includes
#include "BufferedSerial.h"
#include "Thread.h"
#include "mbed.h"
#include "rtos.h"

// constants
#define urmAccount 1
#define MAX_TRIES  40
#define BUAD_RATE  19200

class URM04Sensor {
 private:
  // trigger pin
  DigitalOut _trigPin;
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
  uint64_t managerTimer = 20;
  Timer clock;
  // UART protocol pins
  PinName RX;
  PinName TX;

  // trigger the mesausrements from URM04
  void urmTrigger(int id);
  // reads the distance from URM04
  void urmReader(int id);
  // transmit commands
  void transmitCommands();

  // analyzes the distance**
  void analyzeUrmData(uint8_t cmd[]);
  // runs the sensor
  void runUrm04();
  // decodes URM04 data
  void decodeURM04();

 public:
  // constructor
  URM04Sensor(PinName trig_pin, PinName _RX, PinName _TX);
  // destructor
  ~URM04Sensor();
  void compute_distance();
  void print_distance();
};