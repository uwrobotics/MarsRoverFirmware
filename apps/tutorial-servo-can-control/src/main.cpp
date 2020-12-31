/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"  // header file for microcontroller library
#include "CANMsg.h"
#include "TutorialServo.h"

// declare global objects
CAN can_rx(D4, D10);
CANMsg msg;
TutorialServo servo(PA_1);

// rest of program
int main() {
  while (true)  // loop forever
  {
    if (can_rx.read(msg)) {
      uint8_t extractedMsg = 0;
      // insert CAN packet's data into extractedMsg with >>operator overload, which will convert to an integer bc of uint8_t
      msg >> extractedMsg;
      servo.setPositionInDegrees((extractedMsg / 100) * servo.getServoRangeInDegrees());
    }
  }
}