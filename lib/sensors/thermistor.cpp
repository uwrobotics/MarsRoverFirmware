#include "mbed.h"
#include "thermistor.h"

thermistor::thermistor(PinName thermistor_vout_pin): thermistor_voltage_(thermistor_vout_pin);

float thermistor::readThermistor()
{
    //voltage divider to find resistance of thermister
    //TODO determine what rResistor is - shunt resistance?
    float thermisterResistance = voltage_divider_resistance_ * ((VIN / thermistor_voltage_.read()) - 1);

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
   float measured_thermistor_temp_k = (ROOM_TEMP * B) / (B + (ROOM_TEMP * log((thermisterResistance / THERMISTOR_ROOM_TEMP_RESISTANCE))));

   return measured_thermistor_temp_k - 273.15;
    //return 1/(A + B * log(thermisterResistance) + C * pow(log(thermisterResistance),3)); // uses experimental results
}
