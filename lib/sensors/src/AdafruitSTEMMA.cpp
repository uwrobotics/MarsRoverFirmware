#include "AdafruitSTEMMA.h"

using namespace Sensor;

AdafruitSTEMMA::AdafruitSTEMMA(PinName sda, PinName scl) : m_i2c(sda, scl) {}

AdafruitSTEMMA::AdafruitSTEMMA(const Config &config) : m_i2c(config.sda, config.scl) {}

bool AdafruitSTEMMA::AdafruitSTEMMA::reset() {
  char cmd[3];
  cmd[0] = Sensor_Status_Base;  // initialize registers for clearing sensor memory
  cmd[1] = Sensor_Status_Reset;
  cmd[2] = 0xFF;

  m_i2c.write(Sensor_I2C_Address, cmd, 3);  // set all registers on sensor to default values

  return true;
}

bool AdafruitSTEMMA::AdafruitSTEMMA::getStatus() const {
  char cmd[2];
  cmd[0] = Sensor_Status_Base;
  cmd[1] = Sensor_Status_HW_ID;

  char check[1];

  m_i2c.write(Sensor_I2C_Address, cmd, 2);  // initialize registers for checking device ID
  ThisThread::sleep_for(125ms);
  m_i2c.read(Sensor_I2C_Address, check, 1);  // read device ID

  return (check[0] == Sensor_HW_ID_Code);  // compare received HW ID Code to correct one
}

// read moisture reading of device
bool AdafruitSTEMMA::AdafruitSTEMMA::read(float &sensorReading) {
  if (!(this->getStatus())) {  // checks if device is initialized, returns false if there is an issue
    return false;
  }

  char cmd[2];
  cmd[0] = Sensor_Moisture_Base;
  cmd[1] = Sensor_Moisture_Function;

  char buf[2];

  sensorReading = 65535;

  uint8_t counter = 2;  // initialize counter to break out of loop if reading isn't working (prevent infinite looping)

  do {
    ThisThread::sleep_for(1ms);
    m_i2c.write(Sensor_I2C_Address, cmd, 2);  // initialize registers for reading moisture
    ThisThread::sleep_for(1s);
    m_i2c.read(Sensor_I2C_Address, buf, 2);  // read moisture

    sensorReading = (static_cast<uint16_t>(buf[0]) << 8 | buf[1]);  // concatenate bytes together

    counter--;
  } while (sensorReading == 65535 && counter != 0);  // repeat until value has been measured, or until loop has run 10
                                                     // times (breaks out regardless of if read works or not)

  if (sensorReading == 65535) {
    return false;
  }
  return true;
}

// read temperature of device
bool AdafruitSTEMMA::AdafruitSTEMMA::alternateRead(float &sensorReading) {
  if (!(this->getStatus())) {  // checks if device is initialized, returns false if there is an issue
    return false;
  }

  char cmd[2];
  cmd[0] = Sensor_Status_Base;
  cmd[1] = Sensor_Temp_Function;

  char buf[4];

  m_i2c.write(Sensor_I2C_Address, cmd, 2);  // initialize registers for reading temperature
  ThisThread::sleep_for(1s);
  m_i2c.read(Sensor_I2C_Address, buf, 4);  // read temp

  sensorReading = (static_cast<uint32_t>(buf[0]) << 24) |
                  (static_cast<uint32_t>(buf[1]) << 16) |  // concatenate bytes together
                  (static_cast<uint32_t>(buf[2]) << 8) | static_cast<uint32_t>(buf[3]);
  sensorReading = sensorReading * (1.0 / (1UL << 16));

  return true;
}
