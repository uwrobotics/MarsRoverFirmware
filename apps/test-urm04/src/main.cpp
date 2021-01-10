#include "URM04Sensor.h"
int main() {
  // D2 - trigpin
  // D1 - TX
  // D0 - RX
  URM04Sensor::URM04Sensor sensor(D2, D0, D1);
  float distance_cm;
  while (1) {
    sensor.read_distance(distance_cm);  // measurements returned in Centimeters
    // wait 10 milliseconds
    ThisThread::sleep_for(10ms);

    /************* PRINT DISTANCE **************/
    if (distance_cm > 10000000) {
      printf("Sensor is out of range");
    } else if (distance_cm == -1) {
      printf("ERROR");
    } else {
      printf("Distance: %f cm\n", distance_cm);
    }
  }
  return 0;
}
