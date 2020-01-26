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
    .pin_PWM        = PA_1,
    .degreesPerUnit = 360.0
};

Encoder::t_encoderConfig encRelConfig = {
    .pin_ChannelA           = PA_2,
    .pin_ChannelB           = PA_3,
    .degreesPerUnit         = 0.25,
    .quadratureEncodingType = Encoder::x4_encoding
};

EncoderAbsolute_PWM encAbs(encAbsConfig);
EncoderRelative_Quadrature encRel(encRelConfig);

// main() runs in its own thread in the OS
int main()
{

    while (true) {
        // Blink LED and wait 0.5 seconds
        led1 = !led1;
        wait_ms(500);

        
    }
}
