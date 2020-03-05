#include "mbed.h"
#include "MoistureSensor.h"

DigitalOut led1(LED1);
DigitalOut led2(LED_R);
DigitalOut led3(LED_G);
DigitalOut led4(LED_B);

int main(){

    led1 = 1;
    led2 = 1;
    led3 = 1;
    led4 = 1;

    char* a_data;

    while(1){

        MoistureSensor Stemma(TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL, a_data, 0x36);
        
        if(Stemma.Read_Moisture()){
            led1 = 0;
            led2 = 0;
            led3 = 0;
            led4 = 0;
        }

    }
}