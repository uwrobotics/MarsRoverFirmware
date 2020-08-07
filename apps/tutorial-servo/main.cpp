#include "CANMsg.h"
#include "TutorialServo.h"

CAN can(CAN_RX, CAN_TX, ROVER_CANBUS_FREQUENCY);  // set bit rate to 50kbps

CANMsg rxMsg;

int main() {
  TutorialServo Motor1(PA_1);  // declares an object of type TutorialServo
  float percentage = 0;
  while (1) {
    if (can.read(rxMsg)) {
      // we have recieved a message
      rxMsg.getPayload(percentage);  // recieving a percentage of motion and storing it in data
      Motor1.setPositionInDegrees(
          percentage *
          Motor1.returnAngleRangeInDegrees());  // sets the position to be a percentage of the range of motion
    } else {
      // we have not recieved a message
    }
  }
  return 0;
}
