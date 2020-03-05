#include "mbed.h"
#include "MoistureSensor.h"

DigitalOut led1(LED1);
DigitalOut led2(LED_R);
DigitalOut led3(LED_G);
DigitalOut led4(LED_B);

int main(){

    led1 = 0;                                                                                       //start with all LEDs off
    led2 = 0;
    led3 = 0;
    led4 = 0;

    uint16_t moisture = 0;                                                                          //initialize default variables
    float temp = 0.0;
    bool* init = new bool{false};

    MoistureSensor sensor = MoistureSensor(TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL, init);           //attempt to initialize sensor

    wait_ms(5000);                                                                                  //wait 5 seconds, then turn on LEDs if initialization is successful
    if(*init){
        led1 = 1;
        led2 = 1;
        led3 = 1;
        led4 = 1;
    }

    while(1){        

        wait_ms(1000);

        moisture = sensor.Read_Moisture();                                                          //attempt to read moisture and temperature
        temp = sensor.Read_Temperature();

        //someone who knows how to work with a serial monitor can check these readings

    }
}