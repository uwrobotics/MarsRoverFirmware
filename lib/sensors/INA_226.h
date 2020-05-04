#ifndef SAFETY_BOARD_H
#include SAFETY_BOARD_H
#include "mbed.h"
#include "math.h"
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

class INA_226{
    private:  
        constexpr u_int8_t CONFIG_REGISTER 0x00;
        constexpr u_int8_t VOLTAGE_REGISTER 0x01;
        constexpr u_int8_t CURRENT_REGISTER 0x04;
        constexpr u_int8_t CALIBRATION_REGISTER 0x05;

        //TODO Where do I initialize the serial communication if the application also uses serial?
        Serial pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);    

        //  I2C bus initialization
        I2C i2c_;
        //may need to adjust function to take sensor slave addresses as there many be many current sensors
        u_int8_t sensor_address_ = 0x1c << 1;

        enum CONFIGURATIONMODE{
            powerReset = 0,
            triggered = 3,
            continous = 7
        };

        //below are settings for the INA226 chip. The default values have been selected 
        //but different conversion times can be selected as well. See datasheet for other settings
        constexpr int BUS_VOLTAGE_CONVERSION_TIME = 4; //sets the conversion time for bus voltage measurement(1.1 ms)
        constexpr int SHUNT_VOLTAGE_CONVERSION_TIME = 4; //sets the conversion time for shunt voltage measurement(1.1ms)
        constexpr int AVERAGE_MODE = 0;

        //current conversion constexprants
        float max_expected_current_ = 0; 
        float current_lsb = max_expected_current_ / pow(2,15); 
        float shunt_resistance_ = 0;

    public:
        INA_226(int i2c_frequency, PinName SDA_pinname, PinName SCK_pinname, u_int8_t sensor_address_, float max_expected_current_, float shunt_resistance_);
        ~INA_226();
        float getCurrentData();
        float getVoltageData();
        int calibrateSensor();
        int configureSensor(CONFIGURATIONMODE operation_mode, int reset_registers);
};

#endif /* SAFETY_BOARD_H */

