#include "mbed.h"
#include "MoistureSensor.h"
#include "rover_config.h"

DigitalOut led1(LED1);
DigitalOut led2(LED_R);
DigitalOut led3(LED_G);
DigitalOut led4(LED_B);

Serial     pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);

int main(){

    led1 = 0;                                                                                       //start with all LEDs off
    led2 = 0;
    led3 = 0;
    led4 = 0;

    uint16_t moisture = 0;                                                                          //initialize default variables
    float temperature = 0.0;

    MoistureSensor sensor = MoistureSensor(TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL); 

    wait_ms(5000);                                                                                  
    if(sensor.Is_Initialized()){                                                                    //turn on LEDs if sensor initializes successfully
        led1 = 1;
        led2 = 1;
        led3 = 1;
        led4 = 1;
    }

    while(1){        
        pc.printf("Reading Moisture\r\n");                                                          //read moisture from sensor, reading of 65534 indicates unsuccessful initialization
        moisture = sensor.Read_Moisture(); 
        pc.printf("Moisture: %d \r\n", moisture);

        wait_ms(250);

        pc.printf("Reading Temperature\r\n");                                                       //read temperature from sensor, reading of -273.0 indicates unsuccessful initialization
        temperature = sensor.Read_Temperature();
        pc.printf("Temperature: %f \r\n", temperature);

        wait_ms(1000);
    }
}