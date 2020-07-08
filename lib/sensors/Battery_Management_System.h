#include "mbed.h"
//below are command codes for BMS SMBus communication

typedef enum BMS_COMMAND_CODES
{
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
    AUTHENTICATE = 0x2F, 
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
    SAFETY_ALERT = 0x50,
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

class Battery_management_system
{
private:
    I2C m_I2c;
public:
    Battery_management_system(PinName I2C_SDA_Pin, PinName I2C_SCL_Pin);
    ~Battery_management_system();

    //set BMS parameters
    void setBatteryMode();

    //read BMS values, function types are still tbd as the manual is understood

    float readVoltage();
    float readBatteryStatus();
    float readTemperature();
    float readAvgCurrent();
    float readRemainingCapacity();
    float readTimeToFull();
    float readTimeToEmpty();
    float readChargingStatus();
    float readHealthState();
    float readChargingVoltage();
    float readChargingCurrent();
};
