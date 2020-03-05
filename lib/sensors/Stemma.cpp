#include "mbed.h"
#include "I2C.h"
#include "Stemma.h"

#define I2C_Addr 0x36 << 1                                                //MBED I2C uses 8 bit addressing, so addresses are left shifted by 1 (may need to be shifted by 2)

Stemma::Stemma(bool* initialized){
    char check[1];

    i2c.write(0x00);                                                        //initialize registers for checking device ID
    i2c.write(0x01);

    i2c.read(I2C_Addr, check, 1);                                           //read device ID

    if(check[0] != 0x55){                                                   //compare received HW ID Code to correct one
        *initialized = false;
    }
    else{
        *initialized = true;
    }
}

Stemma::~Stemma(){
}

uint16_t Stemma::Read_Moisture(){
    char buf[2];
    uint16_t ret = 65535;

    do{
        wait_ms(1);

        i2c.write(0x0F);                                                    //initialize registers for reading moisture
        i2c.write(0x10);

        wait_ms(1);

        i2c.read(I2C_Addr, buf, 2);                                         //read moisture

        ret = ((uint16_t)buf[0] << 8 | buf[1]);                             //concatenate bytes together

    } while(ret == 65535);                                                  //repeat until value has been measured

    return ret;
}

float Stemma::Read_Temperature(){
    char buf[4];

    i2c.write(0x00);                                                        //initialize registers for checking temp
    i2c.write(0x04);

    wait_ms(1);

    i2c.read(I2C_Addr, buf, 4);                                             //read temp

    int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |     //concatenate bytes together
                  ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];

    return (1.0 / (1UL << 16)) * ret;
}