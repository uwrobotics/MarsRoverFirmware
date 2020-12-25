/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "TutorialServo.h"
// Blinking rate in milliseconds
#define BLINKING_RATE 500ms
AnalogIn potVoltageIn(PA_0);
PwmOut servoPwmOut(PA_1);
servo1 TutorialServo(PA_1, 180.0, 1, 2);
int main() {
  // Initialise the digital pin LED1 as an output
    
    while(1) {
        servo1.setPositionInDegrees(0);
        wait(0.5);     
        servo1.setPositionInDegrees(180);
        wait(0.5);     
    }
}
