#include "mbed.h"

class MoistureSensor {
    public:

        MoistureSensor(PinName sda, PinName scl, bool* initialized);
        ~MoistureSensor() = default;

        uint16_t Read_Moisture();
        float Read_Temperature();

    private:
        I2C i2c_;
};  