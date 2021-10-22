#include "TutorialServo.h"
#include "mbed.h"

CAN can2(CAN1_RX, CAN1_TX);

TutorialServo servo(PA_1, 180, 1, 2);

int main() {
  CANMessage msg;
  while (1) {
    if (can2.read(msg)) {
      float degrees = servo.getServoRangeInDegrees() * msg;
      servo.setPositionInDegrees(degrees);
    }
  }
}