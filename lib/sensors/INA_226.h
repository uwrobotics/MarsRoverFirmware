#ifndef INA_226_H
#define INA_226_H
#include "mbed.h"
#include "rover_config.h"

// Datasheet for INA226 http://www.ti.com/lit/ds/symlink/ina226.pdf
// current sensor data https://docs.google.com/spreadsheets/d/1Qn4QbEJ1Ia54vYnTgaTvLxHralQvCz6PEGaSF_O1L20/edit#gid=0

//configuration register 00h controls modes of chip
// bit 15 - reset bit, wriitng 1 resets registers. bits 0-2 select operating mode
constexpr u_int8_t CONFIG_REGISTER = 0x00;
constexpr u_int8_t VOLTAGE_REGISTER = 0x01;
constexpr u_int8_t POWER_REGISTER = 0x03;
constexpr u_int8_t CURRENT_REGISTER = 0x04;
constexpr u_int8_t CALIBRATION_REGISTER = 0x05;
constexpr u_int8_t MASK_ENABLE_REGISTER = 0x06;
constexpr u_int8_t ALERT_LIMIT_REGISTER = 0x07;

//configurations for sensor
enum CONFIGURATIONMODE{
    powerReset = 0,
    triggered = 3,
    continous = 7
};

//sample struct for component specific data
struct ComponentConfig
{   
    PinName SDA_pinname; 
    PinName SCL_pinname;
    u_int8_t sensor_address;
    float shunt_resistance;
    float max_expected_current;
};

struct SensorModes{
    u_int8_t bus_voltage_conversion_setting; 
    u_int8_t shunt_voltage_conversion_setting;
    u_int8_t average_mode_setting;
    u_int8_t operation_mode;
    u_int8_t reset_registers;
};

class INA_226{
    private:  
        I2C m_i2c;
        u_int8_t m_sensor_address;

        float m_max_expected_current; 
        float m_current_lsb;
        float m_shunt_resistance;

    public:
        INA_226(ComponentConfig component_config);
        ~INA_226();
        float getCurrentData();
        float getVoltageData();
        float getPowerData();
        int calibrateSensor();
        int configureSensor(SensorModes configuration_bits);
        int setMaskEnableRegister(u_int16_t bits_to_set);
        u_int16_t getAlertLimit();
        int setAlertLimit(u_int16_t alert_limit);

        /*not yet implemented yet
        float getManufacturerID();
        float getDieID();
        */
};

#endif /* INA_226_H */

