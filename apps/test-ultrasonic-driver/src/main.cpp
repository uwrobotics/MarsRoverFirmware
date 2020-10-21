#include <cstdio>

#include "UltrasonicSensor.h"

// set up Ultrasonic sensor
UltrasonicSensor sensor(PA1, PA2);

int main() {
  while (1) {
    printf("Distance: %f cm\n", sensor.getDistance());
  }

  return 0;
}