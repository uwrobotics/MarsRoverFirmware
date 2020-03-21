#include "mbed.h"
#include "CANMsg.h"
#include "math.h"

#define CURRENT_REGISTER 0x04
#define VOLTAGE_REGISTER 0x01
#define CONFIG_REGISTER 0x00

// Datasheet for INA226 http://www.ti.com/lit/ds/symlink/ina226.pdf

// leds on board
DigitalOut led1(LED1);

//  I2C bus initialization
I2C i2c(I2C_SDA, I2C_SCK);
const int SENSOR_SLAVE_ADDRESS = 0x1c << 1;
//configuration register 00h controls modes of chip
// bit 15 - reset bit, wriitng 1 resets registers. bits 0-2 select operating mode
// shunt voltage register 01h
// bus voltage register 02h
// power register 03h
//current register 04h
//calibration register 05h
// mask/enable register 06h
//alert limit register 07h

enum configurationMode{
    powerReset = 0,
    triggered = 3,
    continous = 7
};

//below are settings for the INA226 chip. The default values have been selected 
//but different conversion times can be selected as well. See datasheet for other settings
const int busVoltageConversionTime = 4;
const int shuntVoltageConversionTime = 4;
const int averageMode = 0;


// temperature conversion constants
const float A = 0;
const float B = 0;
const float C = 0;
const float rresistor = 0;

//measurement thresholds
float currentUpperLim = 1;
float currentLowerLim = 0;
float voltageUpperLim = 1;
float voltageLowerLim = 0;
float tempUpperLim = 1;
float tempLowerLim = 0;

//funcs
int inRange(float upperLim, float lowerLim, float val);
float processCurrentData();
int processVoltageData();
float readThermosistor();
status calibrateSensor();
status configureSensor(configurationMode operationMode, int resetRegisters);


int main()
{
    // setup bus
    i2c.frequency(10000);
    // turn led off, turn on if issue occurs
    led1 = 0;

    while (true) {
        float measuredCurrent = getCurrentData();
        if (inRange(currentUpperLim, currentLowerLim, measuredCurrent))
        {
            led1 = 1;
        }


        float measuredTemp = readThermosistor();
        if(inRange(tempUpperLim, tempLowerLim, measuredTemp))
        {
            led1 = 1;
        }
    }
}

int inRange(float upperLim, float lowerLim, float val)
{
    if (val > upperLim || val< lowerLim) ? return 1: return 0;
}


float getCurrentData()
{
    //some conversion factor for current data
    float conversionFactor = 1;
    char cmd[3] = {CURRENT_REGISTER};
    
    i2s.write(SENSOR_SLAVE_ADDRESS, cmd, 2);
    i2c.read(SENSOR_SLAVE_ADDRESS, cmd, 2, false);
    
    char16_t currentData = cmd[1]; 
    currentData = (cmd[2] << 8 | 0x00);
    
    return currentData * conversionFactor;
}


float getVoltageData()
{
    //some conversion factor for voltage data
    float conversionFactor = 1;
    char cmd[3] = {VOLTAGE_REGISTER};
    
    i2s.write(SENSOR_SLAVE_ADDRESS, cmd, 2);
    i2c.read(SENSOR_SLAVE_ADDRESS, cmd, 2, false);
    
    char16_t voltageData = cmd[1]; 
    voltageData = (cmd[2] << 8 | 0x00);
    
    return voltageData * conversionFactor;
}

int configureSensor(configurationMode operationMode, int resetRegisters)
{
    char cmd[3] = {CONFIG_REGISTER, 0x00, 0x00};
    int dataByte = 0x00;

    //fill up register with configuration data
    switch (operationMode)
    {
    case powerReset:
        dataByte |= powerReset;
        break;
    case continous:
        dataByte |= continous;
        break;
    case triggered:
        dataByte |= triggered;
        break;
    default:
        break;
    }
    
    // averaging mode default is 0 so not setting is needed
    dataByte |= (shuntVoltageConversionTime << 3);
    dataByte |= (busVoltageConversionTime << 6);
    
    if (resetRegisters)
    {
        dataByte |= (resetRegisters << 15);
    }
    
    // give cmd the data to write 
    cmd[1] |= dataByte;
    cmd[2] |= dataByte >> 8;

    i2c.write(SENSOR_SLAVE_ADDRESS, cmd, 2);
}


float readThermosistor()
{
    //voltage divider
    float rthermo = rresistor *((vin/vout)-1);

    //convert resistance to temp (steinhart and hart eqn)
    float temperature = 1/(A+B*log(rthermo)+C*exp(log(rthermo),3));
    return temperature;
}