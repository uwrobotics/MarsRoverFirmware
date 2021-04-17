#include "SMBus.h"
#include "math.h"
#include "mbed.h"

/*
 * Will not be using the Sensor class as this Chip is much more complex than most sensors
 * This chip contains many different readings and configuration which is not able to be handled with the sensor parent
 * class
 */

namespace BQ40Z80 {
#define SBS_MANUFACTURER_ACCESS       0x00
#define SBS_MANUFACTURER_BLOCK_ACCESS 0x44
#define MAC_DATA_BUF_SIZE             32

/** 
 * @brief 
 * For UW robotics the battery is 6 cells in series
 */
constexpr int MAX_NUM_CELLS = 6;

/** 
 * @brief 
 * Manufacturer Access command codes
 */
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

  LIFETIME_DATA_FLUSH = 0x002E,

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
  CB_STATUS_MA,
  STATE_OF_HEALTH_MA,
  DA_STATUS_3 = 0X007B,

} SBS_MA_CMD;

/** 
 * @brief 
 * Normal SBS command codes
 */
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
  STATE_OF_HEALTH_DESIGN = 0x4F,
  SAFETY_ALERT           = 0x50,
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
 * @brief 
 * Dataflash command codes for writing to chip registers
 */
typedef enum DATAFLASH_COMMAND_CODES{
  ENABLE_PROTECTIONS_A = 0x4BBE,
} BQ_DATAFLSH_CMD;

/**
 * @brief 
 * a class to configure and control the Battery Management system
 * Will be a central hub of data retrieval and control of BMS, also will calculate values to
 * print to LCD and SD card logging
 */
class BQ40Z80 {
 private:
  /**
   * @param m_address
   * I2C address of chip
   */
  uint16_t m_address;

  /**
   * @param m_smbus
   * smbus interface
   */
  SMBus::SMBus m_smbus;

  /**
   * keys to send to unseal BQ40Z80
   * default is 0x0414, 0x3672
   */
  uint16_t m_keys[2];

  /**
   * @param m_cell_voltages
   * Array storing the voltages of each cell
   */
  uint16_t m_cell_voltages[MAX_NUM_CELLS];

  /** 
   * @brief 
   *
   * @param m_cell_currents
   * Array storing the last known cell current
   * of each cell
   */
  uint16_t m_cell_currents[MAX_NUM_CELLS];

  /** 
   * @brief 
   *
   * @param m_cell_powers
   * Array storing last calculated cell power
   */
  uint16_t m_cell_powers[MAX_NUM_CELLS];

  /** 
   * @brief 
   *
   * @param m_total_power
   * Sum of power of all cells
   */
  uint16_t m_total_power;

  /** 
   * @brief 
   *
   * @param m_avg_power
   * Average power of all cells
   */
  uint16_t m_avg_power;

  /** 
   * @brief 
   *
   * @param m_firmware_version
   * Firmware Version of BQ40Z80 chip
   */
  uint64_t m_firmware_version;

  /** 
   * @brief 
   *
   * @param m_state_of_health
   * Manufacturer Definition of health of cells
   * Describes how much of cells have been permanently used
   */
  uint16_t m_state_of_health;

  /** 
   * @brief 
   *
   * @param m_cycle_count
   * Number of charging cycles
   */
  uint16_t m_cycle_count;

  /** 
   * @brief 
   *
   * @param m_device_type
   * Device type of chip
   */
  uint16_t m_device_type;

  /** 
   * @brief 
   *
   * @param m_remaining_capacity
   * Remaining capacity in V
   */
  uint16_t m_remaining_capacity;

  /** 
   * @brief 
   *
   * @param m_full_charge_capacity
   * Full possibly charge in V
   */
  uint16_t m_full_charge_capacity;

  /** 
   * @brief 
   * 
   * @param m_temp_1
   * Last temp reading from temp sensor 1
   */
  uint16_t m_temp_1;

 public:
  /**
   * @brief Constructor for BMS class
   *
   * @param[in] I2C_SDA_Pin  pinname for SDA pin
   * @param[in] I2C_SCL_Pin  pinname for SCL pin
   */
  BQ40Z80(PinName sda, PinName scl, uint8_t addr);

  /**
   * @brief Distructor for BMS class
   * Not sure if a customer destructor is needed
   */
  ~BQ40Z80() = default;

  /** 
   * @brief 
   *
   * stores status of all cells 
   * @return 0 if successful 
   */
  int getAllCellStatus();

  /** 
   * @brief 
   * Gets some important information on battery state 
   * @return 0 if successful
   */
  int getStartupInfo();

  /* Manufacturer Access Commands
   *
   * Special commands which require a specific command
   * code to be sent. Intended for manufacturing testing but 
   * has useful functions
   */

  /** 
   * @brief 
   * 
   * Gets all cell temperatures
   * @return  0 if successful
   */
  int getTemperatures();

  /** 
   * @brief 
   * 
   * CURRENTLY UNIMPLEMENTED 
   * Seals the chip from incoming SBS commands 
   *  @return 0 if successful
   */ 
  int sealFlashing();

  /** 
   * @brief 
   * 
   * CURRENTLY UNIMPLEMENTED 
   * Allows SBS commands to interact with chip
   * @return 0 if successful
   */
  int unsealFlashing();

  /** 
   * @brief 
   *
   * Reads data from chip using MF command 
   * @param sbs_cmd command to send
   * @param data ptr to store data
   * @param length Number of bytes to read
   * 
   * @return 0 if successful
   */
  int manufacturer_read(const uint16_t sbs_cmd, void *data, const uint8_t length);

  // will send manufacturere access command 0x44 then the data sent is the query for the data command(little endian)
  /** 
   * @brief 
   * 
   * Writes an SBS command using MF access
   * @param sbs_cmd command to write
   * @param data ptr to store data
   * @param length Num bytes to write
   * 
   * @return 0 if successful
   */
  int manufacturer_write(const uint16_t sbs_cmd, void *data, const uint8_t length);

  /** 
   * @brief 
   * 
   * CURRENTLY UNIMPLEMENTED
   * Sets Undervoltage threshold
   * @param avgCurrent
   * 
   * @return 0 if successful
   */
  int setUndervoltageProtection(float avgCurrent);

  /** 
   * @brief 
   * 
   * Sends command to flush lifetime data from chip
   * @return 0 if successful
   */
  int flushLifetimeData();

  /** 
   * @brief 
   * 
   * Reads Life Time data values
   * @return 0 if successful
   */
  int readLifeTimeData();

  // TODO int setShutdownMode();

  /** 
   * @brief 
   * 
   * Retrieves device type from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getDeviceType(uint16_t &data);

  /** 
   * @brief 
   * 
   * Retrieves firmware version from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getFirmwareVersion(uint64_t &data);

  /** 
   * @brief 
   * 
   * Retrieves HW version from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getHardwareVersion(uint16_t &data);

  /** 
   * @brief 
   * 
   * MA command to shutdown the FET's
   * @return 0 if successful
   */
  int enterEmergencyFETShutdown();

   /** 
   * @brief 
   * 
   * MA command to exit shutdown FET mode
   * @return 0 if successful
   */
  int exitEmergencyFETShutdown();

  // MANUFACTURER ACCESS CMDS
  // TODO want to set FET toggles? or should chip handle that?

  // TODO lifetime data handlers eg. delete, speed up, flush

  // TODO safety Alert/status retrieval

  // TODO charging status retrieval

  // TODO Guaging status retrieval

  // TODO Alarm setting for various things EG. capacity

  /* NORMAL CMDS
   *
   * These commands are normal commands sent to the chip
   * They do not have any specific codes like the 
   * Manufacturer Access commands
   */
  
  /** 
   * @brief 
   * 
   * Retrieves Current(MA) from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getCurrent(uint16_t &data);

  /** 
   * @brief 
   * 
   * Retrieves Battery (Charge) mode from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getBatteryMode(uint16_t &data);

  /** 
   * @brief 
   * 
   * Retrieves Battery (Charge) mode from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getBatteryStatus(uint16_t &data);

  /** 
   * @brief 
   * 
   * Retrieves Voltage (V) from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getVoltage(uint16_t &data);

  /** 
   * @brief 
   * 
   * Retrieves Battery (Charge) mode from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getAvgCurrent(uint16_t &data);

  /** 
   * @brief 
   * 
   * Retrieves Time until empty from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getTimeToEmpty(uint16_t &data);

  /** 
   * @brief 
   * 
   * Retrieves Avg time until empty from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getAvgTimeToEmpty(uint16_t &data);
 
  /** 
   * @brief 
   * 
   * Retrieves remaining capacity (V) from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getRemainingCapacity(uint16_t &data);

  /** 
   * @brief 
   * 
   * Retrieves max error of values from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getMaxError(uint16_t &data);

  /** 
   * @brief 
   * 
   * Retrieves battery temperature from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getTemp(uint16_t &data);

   /** 
   * @brief 
   * 
   * Retrieves Relative State of Charge from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
  int getRelativeSOC(uint16_t &data);

    /** 
   * @brief 
   * 
   * Retrieves Absolute State of Charge from chip
   * @param data variable to store result
   * 
   * @return 0 if successful
   */
 int getAbsoluteSOC(uint16_t &data);

  // DATAFLASH CMDS UNUSED AS OF APRIL 17 2021

  /** 
   * @brief 
   * 
   * Reads data from the chip registers. Can
   * also be done using BQstudio
   * @param address Register address
   * @param data ptr to write data to
   * @param length num bytes to read
   * 
   * @return 0 if successful
   */
  int readDataFlash(const uint16_t address, void *data, const unsigned length);

  /** 
   * @brief 
   * 
   * Writes data to the chip registers. Can
   * also be done using BQstudio
   * @param address Register address
   * @param data ptr to write data to
   * @param length num bytes to read
   * 
   * @return 0 if successful
   */
  int writeDataFlash(const uint16_t address, void *data, const unsigned length);
};

}  // namespace BQ40Z80
