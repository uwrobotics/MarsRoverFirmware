#include "math.h"
#include "mbed.h"

// below are command codes for BMS SMBus communication
typedef enum BMS_COMMAND_CODES {
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
  AVERAGE_TIME_TO_EMPTY,
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
} BMSID;

/**
 * @brief a class to configure and control the Battery Management system
 * Will be a central hub of data retrieval and control of BMS, also will calculate values to
 * print to LCD and SD card logging
 */
class BatteryManagementSystem {
 private:
  /**
   * @param m_charging_enabled
   * flag to enable charging
   */
  bool m_charging_enabled;

  /**
   * @param m_charging_disabled
   * flag to disable charging
   */
  bool m_charging_disabled;

  /**
   * @param m_discharging_enabled
   * flag to enable discharging
   */
  bool m_discharging_enabled;

  /**
   * @param m_discharging_disabled
   * flag to disable discharging
   */
  bool m_discharging_disabled;

  /**
   * @param m_enable_sd_card_output
   * may need to be moved to the logging system
   * allows writing to SD card
   */
  bool m_enable_sd_card_output;

  /**
   * @param m_battery_level
   * numerical value of the battery level
   */
  float m_battery_level;

  /**
   * @param m_battery_status
   * Flag with the state fo battery health
   * may need to make a bunch of enums for return statuses for better readability
   */
  int m_battery_status;

  /**
   * @param m_max_current
   * The maximum current read during operation
   */
  float m_max_current;

  /**
   * @param m_I2C
   * mbed I2C pin for SMBus communication
   * is there an SMBus library which we can use instead?
   */
  I2C m_I2c;

  /**
   * @brief Logging functionality
   * Must also be able to use with the SD card
   * Needs to have a good structure
   */
  void logCurrentStatus();

 public:
  /**
   * @brief Constructor for BMS class
   *
   * @param[in] I2C_SDA_Pin  pinname for SDA pin
   * @param[in] I2C_SCL_Pin  pinname for SCL pin
   */
  BatteryManagementSystem(PinName I2C_SDA_Pin, PinName I2C_SCL_Pin);

  /**
   * @brief Distructor for BMS class
   * Not sure if a customer destructor is needed
   */
  ~BatteryManagementSystem() = default;

  /**
   * @brief Misc Control functions for charging and discharging of battery
   * Will update comments for each function
   */
  void enableCharge();
  void disableCharge();
  void enableDischarge();
  void disableDischarge();
  void enablePreCharging();
  void enablePreDischarging();

  /**
   * @brief Calibarte Cell Balancing
   * Needed to keep the discharge and charge of the batteries consistent
   * for battery health
   * not sure how to do this yet, need to read datasheet more
   */
  void calibrateCellBalancing();

  /**
   * @brief Shutdown cells in emergency or otherwise
   * shut down if cells at 3V capacity or is 150A are being drawn
   */
  void shutDownBattery();

  /**
   * @brief set BMS parameters
   * Do whatever to set up the BMS. Willl probably need
   * some parameters to pass in
   */
  void setBatteryMode();

  /**
   * @brief Get standard BMS values
   * Currently included prototypes for the basic functionality
   * function types are still tbd as the manual is understood
   */
  float readCellVoltage();
  float readBatteryStatus();
  float readTemperature();
  float readAvgCurrent();
  float readChargingStatus();

  /**
   * @brief custom bq40z80 commands to retrieve Battery data
   * BMS hopefully calculates custom data for us, may also need more math
   * to use these functions
   * Will create better comments for each function later
   */
  float readRemainingCapacity();
  float readTimeToFull();
  float readTimeToEmpty();
  float readHealthState();
  float readChargingVoltage();
  float readChargingCurrent();

  /**
   * @brief LCD screen functions
   * Controls LCD and prints important information in real time
   * current functions are just prototypes
   */
  void lcdPrintCellVoltages();
  void lcdPrintCriticalCurrent();

  /**
   * @brief SD card logging
   * logs important battery data for custom analysis
   */
  void logToSdCard();
};
