#include "URM04Sensor.h"
int main() {
  // D2 - trigpin
  // D1 - TX
  // D0 - RX
  URM04Sensor::URM04Sensor sensor(D2, D0, D1);
  float get_distance;
  while (1) {
    sensor.read_distance(get_distance);
    // wait 10 milliseconds
    ThisThread::sleep_for(10ms);
    printf("Distance: %f cm\n", get_distance);
  }

  return 0;
}