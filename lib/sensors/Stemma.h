#include "mbed.h"
#include "I2C.h"

#define sda TEMP_MOIST_I2C_SDA                                        //define i2c pins
#define scl TEMP_MOIST_I2C_SCL

class Stemma {
    public:

        Stemma(bool* initialized);
        ~Stemma();

        uint16_t Read_Moisture();
        float Read_Temperature();

    private:
        I2C i2c = I2C(sda, scl);
};  