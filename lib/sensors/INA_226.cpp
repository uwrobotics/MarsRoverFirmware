#include "INA_226.h"

// Datasheet for INA226 http://www.ti.com/lit/ds/symlink/ina226.pdf
// current sensor data https://docs.google.com/spreadsheets/d/1Qn4QbEJ1Ia54vYnTgaTvLxHralQvCz6PEGaSF_O1L20/edit#gid=0

constexpr float INA_226_CALIBRATION_REGISTER_CONSTANT = 0.00512;
constexpr float INA_226_CURRENT_REGISTER_LSB = 0.001; //Amps
constexpr float INA_226_VOLTAGE_REGISTER_LSB = 0.00125; //Volts
constexpr float INA_226_POWER_REGISTER_LSB = 25 * INA_226_CURRENT_REGISTER_LSB; //Watts, Power LSB is 25 times Current LSB
constexpr int CURRENT_LSB_FACTOR = 32768; // factor is equivalent to 2^15

INA_226::INA_226(ComponentConfig component_config): m_i2c(component_config.SDA_pinname, component_config.SCL_pinname)
{
    m_sensor_name = component_config.sensor_name;
    m_max_expected_current = component_config.max_expected_current;
    m_current_lsb = m_max_expected_current / CURRENT_LSB_FACTOR;

    m_shunt_resistance = component_config.shunt_resistance;
    m_sensor_address = component_config.sensor_address << 1; //7 bit address
}

INA_226::~INA_226(){}

float INA_226::getCurrentData()
{
    char cmd[2] = {CURRENT_REGISTER, 0x00};
    
    m_i2c.write(m_sensor_address, cmd, 1);
    m_i2c.read(m_sensor_address, cmd, 2, false);
    
    u_int16_t current_data = (cmd[1] << 8 | cmd[0]);

    return current_data * INA_226_CURRENT_REGISTER_LSB; //multiply by 1 mA/bit to get Amps
}

float INA_226::getVoltageData()
{
    char cmd[2] = {VOLTAGE_REGISTER, 0x00};
    
    m_i2c.write(m_sensor_address, cmd, 1);
    m_i2c.read(m_sensor_address, cmd, 2, false);
    
    u_int16_t voltage_data = (cmd[1] << 8 | cmd[0]);
    
    return voltage_data * INA_226_VOLTAGE_REGISTER_LSB; //multiply by 1.25mV/bit to get Volts
}

float INA_226::getPowerData()
{
    char cmd[2] = {POWER_REGISTER};
    
    m_i2c.write(m_sensor_address, cmd, 1);
    m_i2c.read(m_sensor_address, cmd, 2, false);
    
    u_int16_t power_data = (cmd[1] << 8 | cmd[0]);
    
    return power_data * INA_226_POWER_REGISTER_LSB; //multiply by 25 * current lsb to get Watts
}


//possible modes to configure:
//operation mode -> bits 0-2, default is 0111 continous
//shunt voltage conversion time -> bits 3-5, default is 1.1ms
//bus voltage conversion time -> bits 6-8, default is 1.1 ms
//average mode(number of averages for each sample) -> bits 9-11, default is 1 average
//reset bit (resets register values) -> bit 15, default is 0
//see datasheet for all possible configurations
int INA_226::configureSensor(SensorModes configuration_bits)
{
    char cmd[3] = {CONFIG_REGISTER, 0x00, 0x00};
    u_int16_t dataByte = 0x00;

    dataByte |= configuration_bits.operation_mode; 
    dataByte |= configuration_bits.shunt_voltage_conversion_setting << 3;
    dataByte |= configuration_bits.bus_voltage_conversion_setting << 6;
    dataByte |= configuration_bits.average_mode_setting << 9;
    dataByte |= configuration_bits.reset_registers << 15;
    
    cmd[1] = dataByte & 0xFF;
    cmd[2] = dataByte >> 8 & 0xFF;

    m_i2c.write(m_sensor_address, cmd, 3);
    return 0;
}

int INA_226::calibrateSensor()
{
    int cal = INA_226_CALIBRATION_REGISTER_CONSTANT / (m_current_lsb * m_shunt_resistance);

    char cmd[3] = {CALIBRATION_REGISTER, 0x00, 0x00};

    cmd[1] = cal & 0xFF;
    cmd[2] = cal >> 8 & 0xFF;

    m_i2c.write(m_sensor_address, cmd, 3);
    
    return 0;
}

//setting any of the following bits high will enable the alert pin's functions
//bit 15 -> shunt over voltage -> asserts if shunt voltage exceeds alert limit register
//bit 14 -> shunt under voltage -> asserts if shunt voltage drops below alert limit register
//bit 13 -> bus over voltage -> asserts if bus voltage exceeds alert limit register
//bit 12 -> bus under voltage -> asserts if bus voltage drops below alert limit register
//bit 11 -> power over limit -> asserts if power exceeds alert limit register
//bit 10 -> conversion ready -> asserts if device is ready for next conversion
//bit 4 -> alert function flag -> In latch mode, alert Function flag clears only when mask/enable register is read 
//                             -> In transparent mode, alert Function flag clears after next conversion which does not result in alert condition
//bit 3 -> conversion ready flag -> this bit is set after conversions, averages, multiplications. 
//                               -> cleared when writing config register or reading mask.enable register
//bit 2 -> Math overflow bit -> set to 1 if arithmetic operation resulted in overflow error
//bit 1 -> alert polarity bit -> 1 = inverted, 0 = normal(default)
//bit 0 -> alert latch enable -> 1 = latch enabled, 0 = transparent(default)
int INA_226::setMaskEnableRegister(u_int16_t bits_to_set)
{
    char cmd[3] = {MASK_ENABLE_REGISTER, 0x00, 0x00};
    cmd[1] = bits_to_set & 0xFF;
    cmd[2] = bits_to_set >> 8 & 0xFF;

    m_i2c.write(m_sensor_address, cmd, 3);

    return 0;
}


//result can be used to compare to the value in the Mask/Enable register to determine if a limit has been exceeded
u_int16_t INA_226::getAlertLimit()
{
    char cmd[2] = {ALERT_LIMIT_REGISTER, 0x00};
    
    m_i2c.write(m_sensor_address, cmd, 1);
    m_i2c.read(m_sensor_address, cmd, 2, false);
    
    u_int16_t alert = (cmd[1] << 8 | cmd[0]);
    
    return alert; 
}

//set any limit to notify if bus or shunt voltage has exceeded
int INA_226::setAlertLimit(u_int16_t alert_limit)
{
    char cmd[3] = {ALERT_LIMIT_REGISTER, 0x00, 0x00};
    cmd[1] = alert_limit & 0xFF;
    cmd[2] = alert_limit >> 8 & 0xFF;

    m_i2c.write(m_sensor_address, cmd, 3);

    return 0;
}

char* INA_226::getSensorName()
{
    return m_sensor_name;
}