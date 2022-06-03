#include "DHT22.h"
#include "Logger.h"
#include "PinNames.h"
#include "mbed.h"

int main() {
  Sensor::DHT sensor(A0);
  while (true) {
    // MBED_ASSERT(sensor.update());

    ThisThread::sleep_for(2000ms);

    if (sensor.update()) {
      printf("\r\nSensor data received\r\n");
    } else {
      printf("\r\nSensor reading failed\r\n");
    }

    ThisThread::sleep_for(1000ms);

    printf("Temperature: %f \r\n", sensor.read());
    printf("Temperature: %f \r\n", sensor.alternateRead());
  }
}