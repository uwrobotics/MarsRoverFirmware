/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "CANMsg.h"
#include "TutorialServo.h"

//Declare global variables
CAN can(CAN1_RX, CAN1_TX);
CANMsg msg;
TutorialServo servo(PA_1);
unit8_t convertedMessage = 0;

// main() runs in its own thread in the OS
int main() 
{

  while (true) 
  {
    if(can.read(msg))
    {
      msg >> convertedMessage; 
      servo.setPositionInDegrees(convertedMessage);
    }
  }
}