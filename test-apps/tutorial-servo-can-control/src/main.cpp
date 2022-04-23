#include "CANMsg.h"
#include "TutorialServo.h"
#include "mbed.h"

// assume RX pin is PB_8 and TX pin is PB_9
CAN can(PB_8, PB_9);

// note that txMsg is not declared since I'm not sending packets
CANMsg rxMsg;

// using 1ms ~ 2ms and 0 ~ 180 degrees as the range mapping (default arguments)
TutorialServo servo(PA_1);

int main() {
  // this float is always < 1
  float rangeOfMotionPercent;

  while (1) {
    can.read(rxMsg);

    rxMsg.getPayload(rangeOfMotionPercent);

    servo.setPositionInDegrees(rangeOfMotionPercent * servo.getServoRangeInDegrees());
  }

  return 0;
}