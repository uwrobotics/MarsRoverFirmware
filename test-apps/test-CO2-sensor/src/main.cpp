#include "PollingSensors.h"
#include "PinNames.h"
#include "Logger.h"

DigitalOut led1(LED1);

int main() {
  Sensor::PollingSensors CO2_sensor(PA_0,PA_1);

  while (1) {

    printf("\r\nReading CO2 Levels...\r\n");

    Utility::logger << "CO2: " << ((unsigned  int) CO2_sensor.C02_monitoring()) << "\r\n";

    printf("\r\n-----------------------------\r\n");

    ThisThread::sleep_for(100ms);
  }
}
