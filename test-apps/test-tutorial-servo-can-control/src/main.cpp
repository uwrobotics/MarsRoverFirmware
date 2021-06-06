#include "CANMsg.h"
#include "TutorialServo.h"
#include "mbed.h"

TutorialServo servo(PA_1);
CAN can(PB_8, PB_9);

int main() {
  CANMsg msg;
  while (true) {
    if (can.read(msg)) {
      float msg_data{0.0};
      msg.getPayload(msg_data);
      float degrees{msg_data * servo.getRangeInDegrees()};
      servo.setPositionInDegrees(degrees);
    }
  }
  return 0;
}
