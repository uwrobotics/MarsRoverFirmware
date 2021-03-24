#include "Logger.h"
#include "mbed.h"
#include "BQ40Z80.h"

Utility::Logger logger;

// Test constants
uint32_t EXPECTED_FIRMWARE_VERSION = 1;
uint32_t EXPECTED_DEVICE_TYPE = 1;
uint32_t EXPECTED_STATUS = 1;
uint32_t CURRENT_THRESH_LOW = 0;
uint32_t CURRENT_THRESH_HIGH = 1;
uint32_t V_THRESH_LOW = 0;
uint32_t V_THRESH_HIGH = 1;
uint32_t TEMP_THRESH_LOW = 0;
uint32_t TEMP_THRESH_HIGH = 0;


int main()
{
 //initialize driver
 
  // temp pin names
  PinName i2c_sda = PA_6;
  PinName i2c_scl = PA_7;
  uint8_t addr = 1100001;

  BQ40Z80 BMS(i2c_sda, i2c_scl, addr); 
  //read firmware version
  
  uint32_t data;
  BMS.getFirmwareVersion(data);
  if(data != EXPECTED_FIRMWARE_VERSION)
  {
    logger << "Incorrect FW" << data << "/n";
  } 

  BMS.getDeviceType(data);
  if(data != EXPECTED_DEVICE_TYPE)
  {
    logger << "Incorrect Device type" << data << "/n";
  }
 
  BMS.getBatteryStatus(data);
  if(data != EXPECTED_STATUS)
  {
    logger << "Incorrect Status" << data << "/n";
  }

  BMS.getCurrent(data);
  if(std::clamp(data, CURRENT_THRESH_LOW, CURRENT_THRESH_HIGH))
  {
    logger << "Current outside threshold" << data << "/n";
  }

  BMS.getVoltage(data);
  if(std::clamp(data, V_THRESH_LOW, V_THRESH_HIGH))
  {
    logger << "Voltage outside threshold" << data << "/n";
  }

  BMS.getTemp(data);
  if(std::clamp(data, TEMP_THRESH_LOW, TEMP_THRESH_HIGH))
  {
    logger << "Temperature outside threshold" << data << "/n";
  }


  logger << "Finished tests"; 

  return 0;
}



