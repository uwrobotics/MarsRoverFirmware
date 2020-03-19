#include "mbed.h"
#include "CANMsg.h"
#include "math.h"

#define CURRENT_REGISTER 0x04
#define VOLTAGE_REGISTER 0x01

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


const int k_interval_ms = 500;

enum configurationMode{

}

// temperature conversion constants
const float A = 0;
const float B = 0;
const float C = 0;
const float rresistor = 0;

/*
requires logic to:
1.read current
2. Read voltage
3. read temperature
4. decide if readings are correct
*/

float processCurrentData();
int processVoltageData();
float readThermosistor();
int calibrateSensor();

int main()
{
    // setup bus
    i2c.frequency(10000);
    // turn led on
    led1 = 1;

    while (true) {

        // analyze current data
        led1 = !led1;



        //analyze voltage 

        //analyze temp

    }
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


float readThermosistor()
{
    //voltage divider
    float rthermo = rresistor *((vin/vout)-1);

    //convert resistance to temp (steinhart and hart eqn)
    float temperature = 1/(A+B*log(rthermo)+C*exp(log(rthermo),3));
    return temperature;
}