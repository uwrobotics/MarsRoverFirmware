#include "FSR.h"

int main() {
  force_sensor::FSR sensor(D0);
  float force;
  while (1) {
    if (sensor.getForce(force) != -1) {
      // wait a bit
      ThisThread::sleep_for(1ms);
      printf("Force: %f Nm \n", force);
    } else
      printf("Force Reading Failed\n");
  }
}
