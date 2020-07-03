#include "MoistureSensor.h"

constexpr int Sensor_I2C_Address =
    0x36 << 1;  // MBED I2C uses 8 bit addressing, so addresses are left shifted by 1 (may need to be shifted by 2)

constexpr int Sensor_Status_Base   = 0x00;  // Base address registers for different modules
constexpr int Sensor_Moisture_Base = 0x0F;

constexpr int Sensor_Status_HW_ID = 0x01;  // Function address register for the sensor's HW ID
constexpr int Sensor_HW_ID_Code   = 0x55;  // Expected value for sensor HW ID

constexpr int Sensor_Moisture_Function = 0x10;  // Function address registers for various modules
constexpr int Sensor_Temp_Function     = 0x04;
constexpr int Sensor_Status_Reset      = 0x7F;

MoistureSensor::MoistureSensor(PinName sda, PinName scl) : i2c_(sda, scl) {}

bool MoistureSensor::Is_Initialized() {
  return (this->Read_HW_ID() == Sensor_HW_ID_Code);  // compare received HW ID Code to correct one
}

void MoistureSensor::Reset_Sensor() {
  char cmd[3];
  cmd[0] = Sensor_Status_Base;  // initialize registers for clearing sensor memory
  cmd[1] = Sensor_Status_Reset;
  cmd[2] = 0xFF;

  i2c_.write(Sensor_I2C_Address, cmd, 3);  // set all registers on sensor to default values
}

uint8_t MoistureSensor::Read_HW_ID() {
  char cmd[2];
  cmd[0] = Sensor_Status_Base;
  cmd[1] = Sensor_Status_HW_ID;

  char check[1];

  i2c_.write(Sensor_I2C_Address, cmd, 2);  // initialize registers for checking device ID
  ThisThread::sleep_for(125ms);
  i2c_.read(Sensor_I2C_Address, check, 1);  // read device ID

  return check[0];
}

uint16_t MoistureSensor::Read_Moisture() {
  if (!(this->Is_Initialized())) {  // checks if device is initialized, returns 65534 if there is an issue
    return 65534;
  }

  char cmd[2];
  cmd[0] = Sensor_Moisture_Base;
  cmd[1] = Sensor_Moisture_Function;

  char buf[2];

  uint16_t ret = 65535;

  uint8_t counter = 10;  // initialize counter to break out of loop if reading isn't working (prevent infinite looping)

  do {
    ThisThread::sleep_for(1ms);
    i2c_.write(Sensor_I2C_Address, cmd, 2);  // initialize registers for reading moisture
    ThisThread::sleep_for(1s);
    i2c_.read(Sensor_I2C_Address, buf, 2);  // read moisture

    ret = ((uint16_t)buf[0] << 8 | buf[1]);  // concatenate bytes together

    counter--;
  } while (ret == 65535 && counter != 0);  // repeat until value has been measured, or until loop has run 10 times
                                           // (breaks out regardless of if read works or not)

  return ret;
}

float MoistureSensor::Read_Temperature() {
  if (!(this->Is_Initialized())) {  // checks if device is initialized, returns -273.0 if there is an issue
    return -273.0;
  }

  char cmd[2];
  cmd[0] = Sensor_Status_Base;
  cmd[1] = Sensor_Temp_Function;

  char buf[4];

  i2c_.write(Sensor_I2C_Address, cmd, 2);  // initialize registers for reading temperature
  ThisThread::sleep_for(1s);
  i2c_.read(Sensor_I2C_Address, buf, 4);  // read temp

  int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |  // concatenate bytes together
                ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];

  return (1.0 / (1UL << 16)) * ret;
}
