#include "Servo.h"
#include "mbed.h"

int main() {
  Servo my_servo(PA_0, Servo::LIM_SERVO, 180.0, 2.0, 1.0);

  while (1) {
  }

  return 0;
}
