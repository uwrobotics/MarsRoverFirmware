#include "math.h"
#include "mbed.h"
#include "SMBus.hpp"

/*
 * Will not be using the Sensor class as this Chip is much more complex than most sensors
 *
 * This chip contains many different readings and configuration which is not able to be handled with the sensor parent class
 */

namespace BQ40Z80 {
#define SBS_MANUFACTURER_ACCESS 0x00
#define SBS_MANUFACTURER_BLOCK_ACCESS 0x44
	
//TODO determine the actual size
#define MAC_DATA_BUF_SIZE 4

constexpr MAX_NUM_CELLS = 6;

typedef enum SBS_MA_COMMAND_CODES {
  DEVICE_TYPE = 0x0001,
  FIRMWARE_VERSION,
  HARDWARE_VERSION,
  INSTRUCTION_FLASH_SIG,
  STATIC_DF_SIG,
  CHEM_ID,
  STATIC_CHEM_DF_SIG,
  ALL_DF_SIG,
  SHUTDOWN_MODE,
  SLEEP_MODE,
  AUTO_CC_OFFSET,
  PDSG_FET_TOGGLE,
  FUSE_TOGGLE,
  PCHG_FET_TOGGLE,
  CHG_FET_TOGGLE,
  DSG_FET_TOGGLE,
  GUAGING,
  FET_CONTROL,
  LIFETIME_DATA_COLLECTION,
  PERMANENT_FAILURE,
  BLACK_BOX_RECORDER,
  FUSE,
  LED_DISPLAY_ENABLE,

  LIFETIME_BLK_1 = 0X0060,
  LIFETIME_BLK_2,
  LIFETIME_BLK_3,
  LIFETIME_BLK_4,
  LIFETIME_BLK_5,

  DA_STATUS_1 = 0X0071,
  DA_STATUS_2,
  GUAGING_STATUS_1,
  GUAGING_STATUS_2,
  GUAGING_STATUS_3,
  CB_STATUS,
  STATE_OF_HEALTH,
  DA_STATUS_3 = 0X007B,

} SBS_MA_CMD; 

// below are command codes for BMS SMBus communication
typedef enum SBS_COMMAND_CODES {
  REMAINING_CAPACITY_ALARM = 0x01,
  REMAINING_TIME_ALARM,
  BATTERY_MODE,
  AT_RATE,
  AT_RATE_TIME_TO_FILL,
  AT_RATE_TIME_TO_EMPTY,
  AT_RATE_OK,
  TEMPERATURE,
  VOLTAGE,
  CURRENT,
  AVERAGE_CURRENT,
  MAX_ERROR,
  RELATIVE_STATE_OF_CHANGE,
  ABS_STATE_OF_CHANGE,
  REMAINING_CAPACITY,

  FULL_CHARGE_CAPACITY = 0x10,
  RUN_TIME_TO_EMPTY,
  AVERAGE_TIME_TO_EMPTY,
  AVERAGE_TIME_TO_FULL,
  CHARGING_CURRENT,
  CHARGING_VOLTAGE,
  BATTERY_STATUS,
  CYCLE_COUNT,
  DESIGN_CAPACITY,
  DESIGN_VOLTAGE,
  SPECIFICATION_INFO,
  MANUFACTURER_DATE,
  SERIAL_NUMBER,
  MANUFACTURER_NAME = 0x20,
  DEVICE_NAME,
  DEVICE_CHEMISTY,
  MANUFACTURER_DATA,
  AUTH_CHALLENG = 0x27,
  AUTH_RESPONSE,
  AUTHENTICATE  = 0x2F,
  CELL_VOLTAGE7 = 0x3C,
  CELL_VOLTAGE6,
  CELL_VOLTAGE5,
  CELL_VOLTAGE4,
  ADC1_READ = 0x46,
  ADC2_READ,
  GPIO_READ,
  GPIO_WRITE,
  BTP_DISCHARGE_SET,
  BTP_CHARGE_SET,
  STATE_OF_HEALTH = 0x4F,
  SAFETY_ALERT    = 0x50,
  SAFETY_STATUS,
  PF_ALERT,
  PF_STATUS,
  OPERATION_STATUS,
  CHARGING_STATUS,
  GUAGING_STATUS,
  MANUFACTURING_STATUS,
  AFE_REGISTER,
  MAX_TURBO_POWER,
  SUSTAINED_TURBO_POWER,
  TURBO_PACK_POWER,
  TURBO_SYSTEM_RESISTANCE,
  TURBO_EDV,
  MAX_TURBO_CURR,
  SUSPENDED_TURBO_CURRENT,
  LIFETIME_DATA_BLOCK = 0x60,
  LIFETIME_DATA_BLOCK2,
  LIFETIME_DATA_BLOCK3,
  LIFETIME_DATA_BLOCK4,
  LIFETIME_DATA_BLOCK5,
  MANUFACTURER_INFO = 0x70,
  DA_STATUS1,
  DA_STATUS2,
  GUAGING_STATUS1,
  GUAGING_STATUS2,
  GUAGING_STATUS3,
  CB_STATUS,
  STATE_OF_HEALTH,
  FILTERED_CAPACITY,
  MANUFACTURERE_INFO_B = 0x7A,
  DA_STATUS3,
  GUAGING_STATUS4,
  GUAGING_STATUS5,
  MANUFACTURING_INFO_C = 0x80,
  MANUFACTURING_INFO_D,
  CURRENT_LONG,
} SBS_CMD;

/**
 * @brief a class to configure and control the Battery Management system
 * Will be a central hub of data retrieval and control of BMS, also will calculate values to
 * print to LCD and SD card logging
 */
class BQ40Z80 {
 private:
  /**
   * @param m_I2C
   * mbed I2C pin for SMBus communication
   * is there an SMBus library which we can use instead?
   */
  I2C m_I2c;

  /**
   * @param m_address
   * I2C address of chip
   */
  uint16_t m_address;

  /**
   * @param m_smbus 
   * smbus interface
   */
   SMBus m_smbus;

  /**
   * keys to send to unseal BQ40Z80
   * default is 0x0414, 0x3672 
   */
   uint16_t m_keys[2];

   float m_cell_voltages[MAX_NUM_CELLS];

   float m_cell_currents[MAX_NUM_CELLS];

   float m_cell_powers[MAX_NUM_CELLS];

   float m_total_power;

   float m_avg_power;

   float m_firmware_version;

   float m_state_of_health;
   
   float m_cycle_count;
             
   float m_device_type;

   float m_remaining_capacity;

   float m_full_charge_capacity;

   float m_temp_1;

 public:
  /**
   * @brief Constructor for BMS class
   *
   * @param[in] I2C_SDA_Pin  pinname for SDA pin
   * @param[in] I2C_SCL_Pin  pinname for SCL pin
   */
  BQ40Z80(PinName I2C_SDA_Pin, PinName I2C_SCL_Pin, uint16_t addr);

  /**
   * @brief Distructor for BMS class
   * Not sure if a customer destructor is needed
   */
  ~BQ40Z80() = default;
  
  int getAllCellStatus();

  int getTemperatures();

  int getStartupInfo();

  int sealFlashing();

  int unsealFlashing();

  int manufacturer_read(const uint16_t sbs_cmd, uint32_t &data, const uint8_t length);

  // will send manufacturere access command 0x44 then the data sent is the query for the data command(little endian)
  int manufacturer_write(const uint16_t sbs_cmd, uint32_t &data, const uint8_t length);

  //handled in BQ studio?
  int setUndervoltageProtection(float avgCurrent);

  int flushLifetimeData();

  int readLifeTimeData();

  int setShutdownMode();

  int getDeviceType(uint32_t &data);
 
  int getFirmwareVersion(uint32_t &data);

  int getHardwareVersion(uint32_t &data);

  int enterEmergencyFETShutdown();

  int exitEmergencyFETShutdown();

//MANUFACTURER ACCESS CMDS
  //TODO want to set FET toggles? or should chip handle that?

  //TODO lifetime data handlers eg. delete, speed up, flush
  
  //TODO safety Alert/status retrieval
  
  //TODO charging status retrieval

  //TODO Guaging status retrieval

  //TODO Alarm setting for various things EG. capacity

//NORMAL CMDS
  int getCurrent(uint32_t & data);

  int getBatteryMode(uint16_t & data);
  
  int getBatteryStatus(uint16_t &data);

  int getVoltage(uint32_t & data);

  int getAvgCurrent(uint32_t & data);

  int getTimeToEmpty(uint32_t & data);

  int getAvgTimeToEmpty(uint32_t & data);

  int getRemainingCapacity(uint32_t & data);

  int getMaxError(uint32_t & data);

  int getTemp(uint32_t & data);

  //SOC = state of charge
  int getRelativeSOC(uint32_t & data);

  int getAbsSOC(uint32_t & data);
};
