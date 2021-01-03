/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CANMsg.h"
#include "TutorialServo.h"
#include "mbed.h"

// Declare global variables
CANBus can(CAN1_RX, CAN1_TX);
CANMsg msg;
TutorialServo servo(PA_1);
uint8_t convertedMessage = 0;

// main() runs in its own thread in the OS
int main() {
  while (true) {
    if (can.read(msg)) {
      msg >> convertedMessage;
      servo.setPositionInDegrees(convertedMessage);
    }
  }
}