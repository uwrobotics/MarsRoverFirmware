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
#define CALIBRATION_REGISTER 0x05

// Datasheet for INA226 http://www.ti.com/lit/ds/symlink/ina226.pdf

// leds on board
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

//Analog input from temperature sensor
AnalogIn thermosistorvoltage(TEMP_ANLG_IN);

//Serial communications for debugging
Serial pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);

//CAN initialization
CAN can(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY); // dont know the actual CAN id's
CANMsg rxMsg;
//needs a specific CANID to update values
const unsigned int EXPECTEDCANID = 0x000;

//  I2C bus initialization
I2C i2c(CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL); //pinnames found from pinnames.h
const int SENSOR_SLAVE_ADDRESS = 0x1c << 1;

enum CONFIGURATIONMODE{
    powerReset = 0,
    triggered = 3,
    continous = 7
};

//below are settings for the INA226 chip. The default values have been selected 
//but different conversion times can be selected as well. See datasheet for other settings
const int BUS_VOLTAGE_CONVERSION_TIME = 4; //sets the conversion time for bus voltage measurement(1.1 ms)
const int SHUNT_VOLTAGE_CONVERSION_TIME = 4; //sets the conversion time for shunt voltage measurement(1.1ms)
const int AVERAGE_MODE = 0;

//current conversion constants
const float MAX_EXPECTED_CURRENT = 0; 
float CURRENT_LSB = MAX_EXPECTED_CURRENT / pow(2,15); 

// temperature conversion constants
const float B = 4700; //kelvins. Based on Resistances of 25 and 50 
const float VIN = 5;
//need expected room temp. 22 celcius?
const float ROOM_TEMP = 0;
const float REFERENCE_RESISTANCE = 0;

//measurement thresholds, needs updating
const float CURRENT_UPPER_LIM = 1;
const float CURRENT_LOWER_LIM = 0;
const float CURVOLTAGE_UPPER_LIM = 1;
const float VOLTAGE_LOWER_LIM = 0;
const float TEMP_UPPER_LIM = 1;
const float TEMP_LOWER_LIM = 0;

//funcs
int inRange(float upperLim, float lowerLim, float val);
float getCurrentData();
float getVoltageData();
float readThermistor();
int calibrateSensor();
int configureSensor(CONFIGURATIONMODE operationMode, int resetRegisters);
void initCAN();
void ValidateCANMsg(CANMsg *p_newMsg);

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
            ValidateCANMsg(&rxMsg);
            rxMsg >> command;
            rxMsg.clear();
        }
        //going to need more functionality to select what to measure

        float measuredCurrent = getCurrentData();
        if (inRange(CURRENT_UPPER_LIM, CURRENT_LOWER_LIM, measuredCurrent))
        {
            led1 = 1;
            pc.printf("Measured Current is within expected limits.\r\n");
        }

        float measuredVoltage = getVoltageData();
        if (inRange(CURVOLTAGE_UPPER_LIM, VOLTAGE_LOWER_LIM, measuredVoltage))
        {
            led2 = 1;
            pc.printf("Measured Voltage is within expected limits.\r\n");
        }

        float vout = thermosistorvoltage.read();
        float measuredTemp = readThermistor(VIN, vout);
        if(inRange(TEMP_UPPER_LIM, TEMP_LOWER_LIM, measuredTemp))
        {
            led3 = 1;
            pc.printf("Measured Temperature is within expected limits.\r\n");
        }

        //shutoff all LED's for next round of checks after 2 seconds
        wait(2);
        led1 = 0;
        led2 = 0;
        led3 = 0;
    }
}

void initCAN()
{
    // will need new id's 
    can.filter(CAN1_RX, CAN1_TX, ROVER_CANID_MASK, CANStandard);
}

void ValidateCANMsg(CANMsg *p_newMsg)
{
    // PRINT_INFO("Recieved CAN message with ID %X\r\n", p_newMsg->id);
    // The specific can ID for configuring chip is ___
    switch (p_newMsg->id){
        case EXPECTEDCANID:
            pc.printf("Updating INA226 config\r\n");
            break;
        default:
            pc.printf("Recieved unimplemented command\r\n");
            break;
    }
}

//if in range returns 1, otherwise returns 0
int inRange(float upperLim, float lowerLim, float val)
{
    int inRange = (val > upperLim || val< lowerLim) ? 0: 1;
    return inRange;
}


float getCurrentData()
{
    pc.printf("Retrieving Current\r\n");

    //may need char16_t?
    u_int8_t cmd[3] = {CURRENT_REGISTER};
    
    i2c.write(SENSOR_SLAVE_ADDRESS, cmd, 1);
    i2c.read(SENSOR_SLAVE_ADDRESS, cmd, 2, false);
    
    u_int16_t currentData = (cmd[2] << 8 | cmd[1]);

    return currentData;
}


float getVoltageData()
{
    pc.printf("Retrieving Voltage\r\n");

    u_int8_t cmd[3] = {VOLTAGE_REGISTER};
    
    i2c.write(SENSOR_SLAVE_ADDRESS, cmd, 1);
    i2c.read(SENSOR_SLAVE_ADDRESS, cmd, 2, false);
    
    u_int16_t currentData = (cmd[2] << 8 | cmd[1]);
    
    return voltageData;
}


float readThermistor(float vin, float vout)
{
    pc.printf("Retrieving Temperature of Board\r\n");

    //voltage divider to find resistance of thermister
    float thermisterResistance = rResistor *((vin/vout)-1);

    /*  convert resistance to temp (steinhart and hart eqn)
        A,B,C are Stienhart-hart coefficients which depend on the thermistor
        (https://industrial.panasonic.com/cdbs/www-data/pdf/AUA0000/AUA0000C8.pdf)
        and the temperature range of interest. 
        https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation 
        Need to test the thermistor for resistance at 3 different temperatures and use 
        those values to compute A,B,C

        Alernatively use B value from datasheet which is less accurate but does not require experimetnal data
        https://www.allaboutcircuits.com/projects/measuring-temperature-with-an-ntc-thermistor/
    */ 

    return 1/((1/ROOM_TEMP) + (1/B)*log((thermisterResistance/REFERENCE_RESISTANCE))); // uses B from datasheet
    //return 1/(A + B * log(thermisterResistance) + C * pow(log(thermisterResistance),3)); // uses experimental results
}

int configureSensor(CONFIGURATIONMODE operationMode, int resetRegisters)
{
    u_int8_t cmd[3] = {CONFIG_REGISTER, 0x00, 0x00};
    u_int16_t dataByte = 0x00;

    //fill up register with configuration data, can be powered, continous, triggered
    dataByte |= operationMode;
    
    // averaging mode default is 0 so not setting is needed
    dataByte |= (SHUNT_VOLTAGE_CONVERSION_TIME << 3);
    dataByte |= (BUS_VOLTAGE_CONVERSION_TIME << 6);
    
    if (resetRegisters)
    {
        dataByte |= (resetRegisters << 15);
    }
    
    // give cmd the data to write 
    cmd[1] = dataByte & 0xFF;
    cmd[2] = dataByte >> 8 & 0xFF;

    i2c.write(SENSOR_SLAVE_ADDRESS, cmd, 2);
    return 0;
}

//may need to pass in shunt_resistance
int calibrateSensor()
{
    pc.printf("Calibrating INA226\r\n");
    float cal  =0.00512/(CURRENT_LSB * shunt_resistance); //calibration register, multiplied the current register value to get actual current in Amps

    u_int8_t cmd[3] = {CALIBRATION_REGISTER};
    u_int16_t dataByte = 0x00;

    cmd[1] = cal & 0xFF;

    i2c.write(SENSOR_SLAVE_ADDRESS, cmd, 1);
    
    return 0;
}
