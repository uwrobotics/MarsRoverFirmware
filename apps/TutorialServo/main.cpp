/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "CANMsg.h"
#include "TutorialServo.h"

//Declare global variables
CAN can(D3, D4);
CANMsg msg;
uint8_t convertedMessage;
TutorialServo servo(PA_1, 50, 90);

// main() runs in its own thread in the OS
int main() 
{

  while (true) 
  {
    if(can.read(msg))
    {
      msg >> convertedMessage; //write message to a uint8 integer
      servo.setPositionInDegrees(convertedMessage);
    }
  
  }
}

