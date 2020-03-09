#include "mbed.h"
#include "CANMsg.h"
#include "math.h"


// leds on board
DigitalOut led1(LED1);

//  I2C bus initialization
I2C i2c(I2C_SDA, I2C_SCK);
char sensorData[2];
const int SENSOR_SLAVE_ADDRESS = 0x1c;

const int k_interval_ms = 500;

// temperature conversion constants
const float A = 0;
const float B = 0;
const float C = 0;
const float rresistor = 0;

/*
requires logic to:
1.read current
2. Read voltage
3. read temperature
4. decide if readings are correct


*/

float processCurrentData();
int processVoltageData();
float readThermosistor();

int main()
{
    // setup bus
    i2c.frequency(10000);
    // turn led on
    led1 = 1;

    while (true) {
        // analyze current data
        i2c.read(SENSOR_SLAVE_ADDRESS, sensorData, 2, false);
        led1 = !led1;


        //analyze voltage 

        //analyze temp

    }
}


float processCurrentData()
{



}

float readThermosistor()
{
    //voltage divider
    float rthermo=rresistor *((vin/vout)-1);

    //convert resistance to temp (steinhart and hart eqn)
    float temperature = 1/(A+B*log(rthermo)+C*exp(log(rthermo),3));
    return temperature;


}