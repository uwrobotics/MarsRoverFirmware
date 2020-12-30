/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

/*#include "mbed.h"
#include "tutorialservo.h"
    
AnalogIn potVoltageIn(PA_0);
PwmOut servoPwmOut(PA_1);
    
int main() {
    servoPwmOut.period_ms(20);
    while(1) {
        float potVoltage = potVoltageIn.read();
        servoPwmOut.pulsewidth_ms(1 + potVoltage/3.3);
    }
}*/

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

