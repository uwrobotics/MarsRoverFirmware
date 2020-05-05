#ifndef INA_226_H
#include INA_226_H
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
        u_int8_t m_config_register;
        u_int8_t m_voltage_register;
        u_int8_t m_power_register;
        u_int8_t m_current_register;
        u_int8_t m_calibration_register;
        u_int8_t m_mask_enable_register;
        u_int8_t m_alert_limit_register;

        I2C i2c_;
        u_int8_t m_sensor_address;

        // //below are settings for the INA226 chip. The default values have been selected 
        // //but different conversion times can be selected as well. See datasheet for other settings
        // u_int8_t m_bus_voltage_conversion_setting; //sets the conversion time for bus voltage measurement(1.1 ms)
        // u_int8_t m_shunt_voltage_conversion_setting; //sets the conversion time for shunt voltage measurement(1.1ms)
        // u_int8_t m_average_mode_setting;

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

