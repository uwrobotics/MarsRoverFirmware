#include "mbed.h"
#include <I2C.h>
#include "MoistureSensor.h"

MoistureSensor::MoistureSensor(PinName sda, PinName scl, char* data, int address){
    I2C i2c(sda, scl);    
    data_array = data;
    i2c_address = address;
}

MoistureSensor::~MoistureSensor(){
}

bool MoistureSensor::Read_Moisture(){
    if(i2c.read(i2c_address, data_array, 1, false) == 0){
        return 1;
    }
    else{
        return 0;
    }
}