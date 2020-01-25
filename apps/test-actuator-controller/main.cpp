/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "ActuatorController.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "EncoderRelative_Quadrature.h"

DigitalOut led1(LED1);

Encoder::t_encoderConfig encAbsConfig = {
    .pin_PWM = PA_1,
    .degreesPerUnit = 360.0
};

EncoderAbsolute_PWM encAbs(encAbsConfig);

// main() runs in its own thread in the OS
int main()
{

    while (true) {
        // Blink LED and wait 0.5 seconds
        led1 = !led1;
        wait_ms(500);

        
    }
}
