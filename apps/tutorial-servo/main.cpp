#include "TutorialServo.h"
#include "CANMsg.h"
CAN can(CAN_RX, CAN_TX);
CANMsg rxMsg;
CANMsg txMsg;

int main() {
  // declares an object of type TutorialServo called Motor1 to demonstrate that the object works
  TutorialServo Motor1(PA_1);
  Motor1.setAngleRangeInDegrees(200);
  Motor1.setPulsewidthRangeInMs(1, 3);
  Motor1.setPositionInDegrees(90);
  return 0;
}
