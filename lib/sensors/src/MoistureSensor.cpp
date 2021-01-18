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

MoistureSensor::MoistureSensor::MoistureSensor(const Config &config) : m_i2c(config.sda, config.scl) {}

MoistureSensor::MoistureSensor::~MoistureSensor() {}

bool MoistureSensor::MoistureSensor::reset() {
  char cmd[3];
  cmd[0] = Sensor_Status_Base;  // initialize registers for clearing sensor memory
  cmd[1] = Sensor_Status_Reset;
  cmd[2] = 0xFF;

  m_i2c.write(Sensor_I2C_Address, cmd, 3);  // set all registers on sensor to default values

  return true;
}


bool MoistureSensor::MoistureSensor::getStatus() {
  char cmd[2];
  cmd[0] = Sensor_Status_Base;
  cmd[1] = Sensor_Status_HW_ID;

  char check[1];

  m_i2c.write(Sensor_I2C_Address, cmd, 2);  // initialize registers for checking device ID
  ThisThread::sleep_for(125ms);
  m_i2c.read(Sensor_I2C_Address, check, 1);  // read device ID

   return (check[0] == Sensor_HW_ID_Code);  // compare received HW ID Code to correct one
} 

//read moisture reading of device
bool MoistureSensor::MoistureSensor::read(float &sensorReading) {
  if (!(this->getStatus())) {  // checks if device is initialized, returns false if there is an issue
    return false;
  }

  char cmd[2];
  cmd[0] = Sensor_Moisture_Base;
  cmd[1] = Sensor_Moisture_Function;

  char buf[2];
  float i2c_read_val = 0;
  uint8_t counter = 2;  // initialize counter to break out of loop if reading isn't working (prevent infinite looping)

  do {
    ThisThread::sleep_for(1ms);
    m_i2c.write(Sensor_I2C_Address, cmd, 2);  // initialize registers for reading moisture
    ThisThread::sleep_for(1s);
    m_i2c.read(Sensor_I2C_Address, buf, 2);  // read moisture

    i2c_read_val = (static_cast<uint16_t>(buf[0]) << 8 | buf[1]);  // concatenate bytes together

    counter--;
  } while (i2c_read_val == 65535 && counter != 0);  // repeat until value has been measured, or until loop has run 10 times
                                           // (breaks out regardless of if read works or not)

  if(i2c_read_val == 65535 && counter == 0)
  {
  	return false;
  }

  //if succeeded set sensorReading to i2c_read_val
  sensorReading = i2c_read_val;
  return true;
}

//read temperature of device
bool MoistureSensor::MoistureSensor::alternateRead(float &sensorReading) {
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

  sensorReading = (static_cast<uint32_t>(buf[0]) << 24) | (static_cast<uint32_t>(buf[1]) << 16) |  // concatenate bytes together
                (static_cast<uint32_t>(buf[2]) << 8) | static_cast<uint32_t>(buf[3]);
  sensorReading = sensorReading * (1.0 / (1UL << 16));

  return true;
}
