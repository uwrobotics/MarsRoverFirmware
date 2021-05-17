#include "AdafruitSTEMMA.h"
#include "Logger.h"
#include "hw_bridge.h"

DigitalOut led1(LED1);

int main() {
  led1 = 0;  // start with LED off

  Sensor::AdafruitSTEMMA sensor(TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL);

  while (1) {
    MBED_ASSERT(sensor.reset());

    ThisThread::sleep_for(500ms);

    led1 = (sensor.getStatus());  // turn on LED if the sensor's HW_ID code matches the known value

    printf("\r\nChecking Device ID...\r\n");  // read device HW_ID, reading of 85 is expected

    ThisThread::sleep_for(100ms);

    printf("\r\nReading Moisture...\r\n");  // read moisture from sensor, reading of 65534 indicates
    // unsuccessful initialization
    MBED_ASSERT(sensor.update());
    printf("Moisture: %f \r\n", sensor.read());

    ThisThread::sleep_for(100ms);

    printf("\r\nReading Temperature...\r\n");  // read temperature from sensor, reading of -273.0
                                               // indicates unsuccessful initialization
    printf("Temperature: %f \r\n", sensor.alternateRead());

    printf("\r\n-----------------------------\r\n");

    ThisThread::sleep_for(100ms);
  }
}
