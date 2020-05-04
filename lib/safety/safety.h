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

class Safety_Board{
    private:  
        u_int8_t CONFIG_REGISTER 0x00;
        u_int8_t VOLTAGE_REGISTER 0x01;
        u_int8_t CURRENT_REGISTER 0x04;
        u_int8_t CALIBRATION_REGISTER 0x05;

        //TODO Where do I initialize the serial communication if the application also uses serial?
        Serial pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);    

        //  I2C bus initialization
        I2C i2c;
        //may need to adjust function to take sensor slave addresses as there many be many current sensors
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
        const float SHUNT_RESISTANCE = 0;

        // temperature conversion constants
        const float B = 4700; //kelvins. Based on Resistances of 25 and 50 
        const float VIN = 5;
        //need expected room temp. 22 celcius?
        const float ROOM_TEMP = 0;
        const float REFERENCE_RESISTANCE = 0;
    
    public:
        // leds on board
        DigitalOut led1;
        DigitalOut led2;
        DigitalOut led3;
        //Analog input from temperature sensor
        AnalogIn thermistorvoltage;

        //initialize constructor for leds, analog pin, i2c
        Safety_Board(PinName led1PinName, PinName led2PinName, PinName led3PinName, PinName voltagePinName, int i2cFrequency, PinName SDAPinName, PinName SCKPinName);
        ~Safety_Board();
        float getCurrentData();
        float getVoltageData();
        float readThermistor();
        int calibrateSensor();
        int configureSensor(CONFIGURATIONMODE operationMode, int resetRegisters);
};

#endif /* SAFETY_BOARD_H */

