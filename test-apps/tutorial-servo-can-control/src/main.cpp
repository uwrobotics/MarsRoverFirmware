#include "CANInterface.h"
#include "TutorialServo.hpp"
#include "hw_bridge.h"

TutorialServo servo(PA_1);
CANBus can(CAN1_RX, CAN1_TX);

int main() {
  float percent, degrees;
  CANMsg rxMsg;

  while (true) {
    can.read(rxMsg);

    rxMsg.getPayload(percent);
    degrees = servo.getServoRangeInDegrees();
    servo.setPositionInDegrees(degrees * percent);
  }
}