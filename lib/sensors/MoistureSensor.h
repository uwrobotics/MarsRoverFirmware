#include "mbed.h"
#include <I2C.h>

#define I2C_Addr 0x36

class MoistureSensor {
    public:

        MoistureSensor(PinName sda, PinName scl, char* data, int address = I2C_Addr);   //choose the right fucking pins
        ~MoistureSensor();
        bool Read_Moisture();


    private:

        PinName i2c_data;
        PinName i2c_clock;
        char* data_array;
        int i2c_address;
};