#include "PollingSensors.h"
#include "PinNames.h"
#include "Logger.h"

DigitalOut led1(LED1);

int main() {
  Sensor::PollingSensors moisture_sensor(PA_0, PA_1);

  while (1) {

    printf("\r\nReading moisture Levels...\r\n");

    Utility::logger << "Moisture: " << ((unsigned  int) moisture_sensor.moisture_monitoring()) << "\r\n";

    printf("\r\n-----------------------------\r\n");

    ThisThread::sleep_for(100ms);
  }
}
