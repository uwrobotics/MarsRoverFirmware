#include "mbed.h"
#include "MoistureSensor.h"
#include "rover_config.h"

DigitalOut led1(LED1);
Serial     pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);

int main(){

    led1 = 0;                                                                                               //start with LED off

    uint16_t moisture = 0;                                                                                  //initialize default variables
    float temperature = 0.0;
    int HW_ID = 0;

    MoistureSensor sensor = MoistureSensor(TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL); 

    while(1){        
        if(sensor.Is_Initialized()){                                                                        //turn on LED if the sensor's HW_ID code matches the known value
            led1 = 1;                                                                                       //this has been finicky, so don't worry if the LED doesn't turn on but the readings are fine
        }
        else{
            led1 = 0;
        }

        pc.printf("\r\nChecking Device ID...\r\n");                                                         //read device HW_ID, reading of 104 is expected but likely won't cause issues if different
        HW_ID = sensor.Read_HW_ID();
        pc.printf("HW ID: %d \r\n", HW_ID);

        wait_ms(250);

        pc.printf("\r\nReading Moisture...\r\n");                                                           //read moisture from sensor, reading of 65534 indicates unsuccessful initialization
        moisture = sensor.Read_Moisture(); 
        pc.printf("Moisture: %d \r\n", moisture);

        wait_ms(250);

        pc.printf("\r\nReading Temperature...\r\n");                                                        //read temperature from sensor, reading of -273.0 indicates unsuccessful initialization
        temperature = sensor.Read_Temperature();
        pc.printf("Temperature: %f \r\n", temperature);

        pc.printf("\r\n-----------------------------\r\n");

        wait_ms(1000);
    }
}