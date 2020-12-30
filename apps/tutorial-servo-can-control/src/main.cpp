/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
*/

#include "mbed.h" //header file for microcontroller library
#include "TutorialServo.h"

//declare global objects
CAN can_rx(D4, D10);
CANMessage msg;
TutorialServo servo(PA_1);

//rest of program
int main()
{
  while(true) //loop forever
  {
    if(can_rx.read(msg))
      {
        unit8_t extractedMsg = 0;
        msg >> extractedMsg; 
          //>>operator inserts CAN packet's data to extractedMsg, which will convert to an integer because of the unit8_t type

        servo.setPositionInDegrees((extractedMsg/100)*servo.getServoRangeInDegrees());
      }
  }
}

