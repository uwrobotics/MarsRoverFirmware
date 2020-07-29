#include "CANMsg.h"
#include "TutorialServo.h"
#include "mbed.h"

CAN can(D3, D4);
CANMsg msg;
uint8 in;
TutorialServo servo(PA_1, 180.0);

int main() {
  can.frequency(500000);

  while (true) {
    if (can.read(msg)) {
      msg >> in;
    }
    servo.setPositionInDegrees(180 * in / 100);
  }
}
