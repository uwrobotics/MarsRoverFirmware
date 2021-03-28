#include "BQ40Z80.h"
#include "Logger.h"
#include "mbed.h"

Utility::Logger logger;

// Test constants
uint16_t EXPECTED_FIRMWARE_VERSION = 1;
uint16_t EXPECTED_DEVICE_TYPE      = 1;
uint16_t EXPECTED_STATUS           = 1;
uint16_t CURRENT_THRESH_LOW        = 0;
uint16_t CURRENT_THRESH_HIGH       = 1;
uint16_t V_THRESH_LOW              = 0;
uint16_t V_THRESH_HIGH             = 1;
uint16_t TEMP_THRESH_LOW           = 0;
uint16_t TEMP_THRESH_HIGH          = 0;

int main() {
  // initialize driver

  // temp pin names
  PinName i2c_sda = PC_9;
  PinName i2c_scl = PA_8;
  uint8_t addr    = 0b1100001;

  BQ40Z80::BQ40Z80 BMS(i2c_sda, i2c_scl, addr);
  // read firmware version

  uint64_t version;
  BMS.getFirmwareVersion(version);
  if (version != EXPECTED_FIRMWARE_VERSION) {
    logger << "Incorrect FW" << version << "/n";
  }

  uint16_t data;
  BMS.getDeviceType(data);
  if (data != EXPECTED_DEVICE_TYPE) {
    logger << "Incorrect Device type" << data << "/n";
  }

  BMS.getBatteryStatus(data);
  if (data != EXPECTED_STATUS) {
    logger << "Incorrect Status" << data << "/n";
  }

  BMS.getCurrent(data);
  if (std::clamp(data, CURRENT_THRESH_LOW, CURRENT_THRESH_HIGH)) {
    logger << "Current outside threshold" << data << "/n";
  }

  BMS.getVoltage(data);
  if (std::clamp(data, V_THRESH_LOW, V_THRESH_HIGH)) {
    logger << "Voltage outside threshold" << data << "/n";
  }

  BMS.getTemp(data);
  if (std::clamp(data, TEMP_THRESH_LOW, TEMP_THRESH_HIGH)) {
    logger << "Temperature outside threshold" << data << "/n";
  }

  logger << "Finished tests";

  return 0;
}
