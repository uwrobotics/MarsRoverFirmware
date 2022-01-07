#include "CANInterface.h"
#include "hw_bridge.h"
#include "TutorialServo.hpp"

TutorialServo servo(PA_1);
CANBus can(CAN1_RX, CAN1_TX);

int main(){

  float percent;
  CANMsg rxMsg;

  can.read(rxMsg);

  rxMsg.getPayload(percent);
  servo.setPositionInDegrees(percent);
}