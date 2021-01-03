#include "URM04Sensor.h"
int main() {
  // D2 - trigpin
  // D1 - TX
  // D0 - RX
  URM04Sensor::URM04Sensor sensor(D2, D0, D1);
  while (1) {
    sensor.trigger_sensor();
    sensor.read_distance();
    ThisThread::sleep_for(std::chrono::milliseconds(10));
  }    

  return 0;
}