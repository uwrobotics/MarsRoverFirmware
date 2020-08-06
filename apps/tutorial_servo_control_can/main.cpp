#include "CANMsg.h"
#include "TutorialServo.h"
#include "mbed.h"

CAN can(D3, D4);
CANMsg msg;
TutorialServo servo(PA_1, 180.0);

int main() {
  uint in;
  can.frequency(500000);

  while (true) {
    if (can.read(msg)) {
      msg >> in;
    }
    servo.setPositionInDegrees(180 * in / 100);
  }
}
