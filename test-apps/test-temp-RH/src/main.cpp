#include "DHT22.h"
#include "Logger.h"
#include "PinNames.h"
#include "mbed.h"

int main() {
  Sensor::DHT sensor(D7);
  while (true) {
    // MBED_ASSERT(sensor.update());

    ThisThread::sleep_for(2000ms);

    if (sensor.update()) {
      printf("\r\nSensor data received\r\n");

      ThisThread::sleep_for(1000ms);

      printf("Humidity: %i\r\n", 1000 * (int)sensor.read());
      printf("Temperature: %i\r\n", 1000 * (int)sensor.alternateRead());

    } else {
      printf("\r\nSensor reading failed\r\n");
    }
  }
}