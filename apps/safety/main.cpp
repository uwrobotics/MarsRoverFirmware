#include "mbed.h"
#include "CANMsg.h"
#include "math.h"
#include "rover_config.h"

//configuration register 00h controls modes of chip
// bit 15 - reset bit, wriitng 1 resets registers. bits 0-2 select operating mode
// bus voltage register 02h
// power register 03h
//calibration register 05h
// mask/enable register 06h
//alert limit register 07h
#define CONFIG_REGISTER 0x00
#define VOLTAGE_REGISTER 0x01
#define CURRENT_REGISTER 0x04

// Datasheet for INA226 http://www.ti.com/lit/ds/symlink/ina226.pdf

// leds on board
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

//Serial communications for debugging
Serial pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);

//CAN initialization
CAN can(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY); // dont know the actual CAN id's
CANMsg rxMsg;
//needs a specific CANID to update values
const unsigned int expectedCANID = 0x000;

//  I2C bus initialization
I2C i2c(CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL); //pinnames found from pinnames.h
const int SENSOR_SLAVE_ADDRESS = 0x1c << 1;

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
const float rResistor = 0;

//measurement thresholds, needs updating
float currentUpperLim = 1;
float currentLowerLim = 0;
float voltageUpperLim = 1;
float voltageLowerLim = 0;
float tempUpperLim = 1;
float tempLowerLim = 0;

//funcs
int inRange(float upperLim, float lowerLim, float val);
float getCurrentData();
float getVoltageData();
float readThermosistor();
int calibrateSensor();
int configureSensor(configurationMode operationMode, int resetRegisters);
void initCAN();
void processCANMsg(CANMsg *p_newMsg);

int main()
{
    // setup bus
    i2c.frequency(10000);
    initCAN();
    // turn led off, turn on if issue occurs
    led1 = 0;
    led2 = 0;
    led3 = 0;

    while (true) {
        //read can messages to get data to configure device
        u_int8_t command = 0;
        if(can.read(rxMsg))
        {
            processCANMsg(&rxMsg);
            rxMsg >> command;
            rxMsg.clear();
        }

        float measuredCurrent = getCurrentData();
        if (inRange(currentUpperLim, currentLowerLim, measuredCurrent))
        {
            led1 = 1;
        }

        float measuredVoltage = getVoltageData();
        if (inRange(voltageUpperLim, voltageLowerLim, measuredVoltage))
        {
            led2 = 1;
        }

        float measuredTemp = readThermosistor();
        if(inRange(tempUpperLim, tempLowerLim, measuredTemp))
        {
            led3 = 1;
        }
    }
}

void initCAN()
{
    // will need new id's 
    can.filter(CAN1_RX, CAN1_TX, ROVER_CANID_MASK, CANStandard);
}

void processCANMsg(CANMsg *p_newMsg)
{
    // PRINT_INFO("Recieved CAN message with ID %X\r\n", p_newMsg->id);
    // The specific can ID for configuring chip is ___
    switch (p_newMsg->id){
        case expectedCANID:
            pc.printf("Updating INA226 config\r\n");
            break;
        default:
            pc.printf("Recieved unimplemented command\r\n");
            break;
    }
}


int inRange(float upperLim, float lowerLim, float val)
{
    int inRange = (val > upperLim || val< lowerLim) ? 1: 0;
    return inRange;
}


float getCurrentData()
{
    //some conversion factor for current data
    float conversionFactor = 1;
    //may need char16_t?
    char cmd[3] = {CURRENT_REGISTER};
    
    i2c.write(SENSOR_SLAVE_ADDRESS, cmd, 2);
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
    
    i2c.write(SENSOR_SLAVE_ADDRESS, cmd, 2);
    i2c.read(SENSOR_SLAVE_ADDRESS, cmd, 2, false);
    
    char16_t voltageData = cmd[1]; 
    voltageData = (cmd[2] << 8 | 0x00);
    
    return voltageData * conversionFactor;
}


float readThermosistor(float vin, float vout)
{
    //voltage divider
    float rthermo = rResistor *((vin/vout)-1);

    //convert resistance to temp (steinhart and hart eqn)
    return 1/(A + B * log(rthermo) + C * pow(log(rthermo),3));
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
    return 0;
}