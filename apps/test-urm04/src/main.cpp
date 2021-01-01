#include "URM04Sensor.h"

int main() {
  // D2 - trigpin
  // D1 - TX
  // D0 - RX
  URM04Sensor sensor(D2, D0, D1);
  while (1) {
    sensor.compute_distance();
    sensor.print_distance();
  }
  return 0;
}