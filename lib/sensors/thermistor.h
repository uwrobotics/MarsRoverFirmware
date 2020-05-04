#ifndef THERMISTOR_H
#include THERMISTOR_H
#include "mbed.h"
#include "math.h"

class thermistor{
    private:
        constexpr float B = 4700; //kelvins. Based on Resistances of 25 and 50 , part number ERT-J1VV104J
        constexpr float VIN = 5; //might need to be in bytes
        //TODO need to confirm expected room temp. 
        constexpr float ROOM_TEMP = 295.15; //degrees in kelvin - 22 celcius
        constexpr float THERMISTOR_ROOM_TEMP_RESISTANCE = 100000; //ohms
        constexpr float voltage_divider_resistance_ = 100000; //ohms
    public:
    AnalogIn thermistor_voltage_;

    thermistor::thermistor(PinName thermistor_vout_pin);
    ~thermistor();
    float readThermistor();
}

#endif /* THERMISTOR_H */