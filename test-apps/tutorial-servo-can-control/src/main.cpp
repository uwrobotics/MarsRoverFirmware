#include "CANMsg.h"
#include "TutorialServo.h"
#include "mbed.h"

// servo PWM control pin is connected to PA_1
PwmOut servoPwmOut(PA_1);

// assume RX pin is PB_8 and TX pin is PB_9
CAN can(PB_8, PB_9);

// note that txMsg is not declared since I'm not sending packets
CANMsg rxMsg;

int main() {
  // pass no paramter to TutorialServo's constructor, using 1ms ~ 2ms and 0 ~ 180 degrees as the range mapping
  TutorialServo servo = TutorialServo();
  float rangeOfMotionPercent;

  while (1) {
    can.read(rxMsg);

    // a float is four byte, use >> operator to map it directly to rangeOfMotionPercent
    rxMsg >> rangeOfMotionPercent;

    servo.setPositionInDegrees(rxMsg);
  }

  return 0;
}