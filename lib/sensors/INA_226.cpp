#include "mbed.h"
#include "INA_226.h"

INA_226::INA_226(int i2c_frequency, PinName SDA_pinname, PinName SCK_pinname, u_int8_t sensor_address, float max_expected_current, float shunt_resistance)
{
    shunt_resistance_ = shunt_resistance;
    max_expected_current_ = max_expected_current;
    sensor_address_ = sensor_address << 1;

    i2c_(SDA_pinname, SCK_pinname);
    i2c_.frequency(i2c_frequency);
}

INA_226::~INA_226(){}

//TODO: may need to adjust function to take sensor slave addresses as there many be many current sensors
float Safety_Board::getCurrentData()
{
    //may need char16_t?
    u_int8_t cmd[3] = {CURRENT_REGISTER};
    
    i2c_.write(sensor_address_, cmd, 1);
    i2c_.read(sensor_address_, cmd, 2, false);
    
    u_int16_t current_data = (cmd[2] << 8 | cmd[1]);

    return current_data;
}


float Safety_Board::getVoltageData()
{
    u_int8_t cmd[3] = {VOLTAGE_REGISTER};
    
    i2c_.write(sensor_address_, cmd, 1);
    i2c_.read(sensor_address_, cmd, 2, false);
    
    u_int16_t voltage_data = (cmd[2] << 8 | cmd[1]);
    
    return voltage_data;
}

int Safety_Board::configureSensor(CONFIGURATIONMODE operation_mode, int reset_registers)
{
    u_int8_t cmd[3] = {CONFIG_REGISTER, 0x00, 0x00};
    u_int16_t dataByte = 0x00;

    //fill up register with configuration data, can be powered, continous, triggered
    dataByte |= operation_mode;
    
    // averaging mode default is 0 so not setting is needed
    dataByte |= (SHUNT_VOLTAGE_CONVERSION_TIME << 3);
    dataByte |= (BUS_VOLTAGE_CONVERSION_TIME << 6);
    
    if (reset_registers)
    {
        dataByte |= (reset_registers << 15);
    }
    
    cmd[1] = dataByte & 0xFF;
    cmd[2] = dataByte >> 8 & 0xFF;

    i2c_.write(sensor_address_, cmd, 2);
    return 0;
}

//may need to pass in shunt_resistance
int Safety_Board::calibrateSensor()
{
    float cal  = 0.00512/(current_lsb_ * shunt_resistance_); //calibration register, multiplied w/ current register value to get actual current in Amps

    u_int8_t cmd[3] = {CALIBRATION_REGISTER};

    cmd[1] = cal & 0xFF;

    i2c_.write(sensor_address_, cmd, 1);
    
    return 0;
}
