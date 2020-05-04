#include "mbed.h"
#include "safety.h"

Safety_Board::Safety_Board(PinName led1PinName, PinName led2PinName, PinName led3PinName,
                             PinName voltagePinName, int i2cFrequency, PinName SDAPinName, PinName SCKPinName)
{
    //TODO: check pin initializations
    led1(led1PinName);
    led2(led2PinName);
    led3(led3PinName);

    thermistorVoltage(voltagePinName);

    i2c(SDAPinName, SCKPinName);
    i2c.frequency(i2cFrequency);
}

Safety_Board::~Safety_Board(){}

//TODO: may need to adjust function to take sensor slave addresses as there many be many current sensors
float Safety_Board::getCurrentData()
{
    pc.printf("Retrieving Current\r\n");

    //may need char16_t?
    u_int8_t cmd[3] = {CURRENT_REGISTER};
    
    i2c.write(SENSOR_SLAVE_ADDRESS, cmd, 1);
    i2c.read(SENSOR_SLAVE_ADDRESS, cmd, 2, false);
    
    u_int16_t currentData = (cmd[2] << 8 | cmd[1]);

    return currentData;
}


float Safety_Board::getVoltageData()
{
    pc.printf("Retrieving Voltage\r\n");

    u_int8_t cmd[3] = {VOLTAGE_REGISTER};
    
    i2c.write(SENSOR_SLAVE_ADDRESS, cmd, 1);
    i2c.read(SENSOR_SLAVE_ADDRESS, cmd, 2, false);
    
    u_int16_t voltageData = (cmd[2] << 8 | cmd[1]);
    
    return voltageData;
}


float Safety_Board::readThermistor(float vin, float vout)
{
    pc.printf("Retrieving Temperature of Board\r\n");

    //voltage divider to find resistance of thermister
    //TODO determine what rResistor is
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

int Safety_Board::configureSensor(CONFIGURATIONMODE operationMode, int resetRegisters)
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
int Safety_Board::calibrateSensor()
{
    pc.printf("Calibrating INA226\r\n");
    float cal  = 0.00512/(CURRENT_LSB * SHUNT_RESISTANCE); //calibration register, multiplied w/ current register value to get actual current in Amps

    u_int8_t cmd[3] = {CALIBRATION_REGISTER};

    cmd[1] = cal & 0xFF;

    i2c.write(SENSOR_SLAVE_ADDRESS, cmd, 1);
    
    return 0;
}
