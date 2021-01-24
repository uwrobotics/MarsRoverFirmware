#include "CANBuffer.h"
#include "CANBus.h"
#include "CANMsg.h"
#include "hw_bridge.h"
#include "mbed.h"
#include "TutorialServo.h"

PwmOut servoPwmOut(PA_1);

CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

int main() {
  CANMsg rxMsg;
  TutorialServo servo(servoPwmOut);
  while (true) {
    float degrees;
    rxMsg.getPayload(degrees);
    servo.setPositionInDegrees(degrees);
    ThisThread::sleep_for(2ms);
  }
}