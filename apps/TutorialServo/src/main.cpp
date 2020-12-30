/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "CANBus.h"
#include "CANMsg.h"
#include "mbed.h"
#include "TutorialServo.h"

// Blinking rate in milliseconds
// #define BLINKING_RATE 500ms
// AnalogIn potVoltageIn(PA_0);
// PwmOut servoPwmOut(PA_1);

CAN can(CAN1_RX, CAN1_TX);
TutorialServo servo_1(PA_1);
CANMessage msg;

int main() {    
    while(true) {
        if(can.read(msg)){
          servo.setPositionInDegrees(msg.data[0]);
        }
    }
}
