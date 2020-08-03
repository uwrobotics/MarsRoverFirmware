#include "CANMsg.h"
#include "TutorialServo.h"

CAN can(CAN_RX, CAN_TX);
CANMsg rxMsg;
uint8_t counter = 0;

#define TX_ID 0x100
#define RX_ID 0x101

int main() {
  can.frequency(500000);       // set bit rate to 50kbps
  TutorialServo Motor1(PA_1);  // declares an object of type TutorialServo

  while (1) {
    if (can.read(rxMsg)) {
      // we have recieved a message
    } else {
      // we have not recieved a message
    }
  }

  Motor1.setAngleRangeInDegrees(200);
  Motor1.setPulsewidthRangeInMs(1, 3);
  Motor1.setPositionInDegrees(90);
  return 0;
}
