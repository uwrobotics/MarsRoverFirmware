#ifndef INA_226_H
#define INA_226_H
#include "mbed.h"
#include "rover_config.h"

// Datasheet for INA226 http://www.ti.com/lit/ds/symlink/ina226.pdf
// current sensor data https://docs.google.com/spreadsheets/d/1Qn4QbEJ1Ia54vYnTgaTvLxHralQvCz6PEGaSF_O1L20/edit#gid=0

//configuration register 00h controls modes of chip
// bit 15 - reset bit, wriitng 1 resets registers. bits 0-2 select operating mode
// bus voltage register 02h
// power register 03h
//calibration register 05h
// mask/enable register 06h
//alert limit register 07h

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
}

class INA_226{
    private:  
        static constexpr u_int8_t M_CONFIG_REGISTER = 0x00;
        static constexpr u_int8_t M_VOLTAGE_REGISTER = 0x01;
        static constexpr u_int8_t M_POWER_REGISTER = 0x03;
        static constexpr u_int8_t M_CURRENT_REGISTER = 0x04;
        static constexpr u_int8_t M_CALIBRATION_REGISTER = 0x05;
        static constexpr u_int8_t M_MASK_ENABLE_REGISTER = 0x06;
        static constexpr u_int8_t M_ALERT_LIMIT_REGISTER = 0x07;

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
        int setMaskEnableRegister();
        u_int16_t getAlertLimit();
        int setAlertLimit();

        /*not yet implemented yet
        float getManufacturerID();
        float getDieID();
        */
};

#endif /* INA_226_H */

