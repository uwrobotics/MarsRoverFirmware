#include "mbed.h"

class MoistureSensor {
    public:

        MoistureSensor(PinName sda, PinName scl);
        ~MoistureSensor() = default;

        int Read_HW_ID();
        bool Is_Initialized();

        uint16_t Read_Moisture();
        float Read_Temperature();

    private:
        I2C i2c_;
};  