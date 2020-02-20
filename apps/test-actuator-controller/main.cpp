/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "ActuatorController.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "EncoderRelative_Quadrature.h"
#include "ActuatorController.h"

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

ActuatorController::t_actuatorConfig actuatorConfig = {
    .defaultControlMode = ActuatorController::motorPower,

    .minMotorPower_Percentage = -1.0, 
    .maxMotorPower_Percentage = 1.0,
    
    .minVelocity_DegreesPerSec = -5.0, 
    .maxVelocity_DegreesPerSec = 5.0,
    
    .minAngle_Degrees = -90.0, 
    .maxAngle_Degrees = 90.0,

    .velocityPID = {
        .P = 1.0,
        .I = 0.0,
        .D = 0.0
    }, 

    .positionPID = {
        .P = 1.0,
        .I = 0.0,
        .D = 0.0
    }
};

Motor motor(PA_5, PA_6);

ActuatorController actuatorAbs(actuatorConfig, motor, encAbs);
ActuatorController actuatorRel(actuatorConfig, motor, encAbs);

// main() runs in its own thread in the OS
int main()
{

    while (true) {
        // Blink LED and wait 0.5 seconds
        led1 = !led1;
        wait_ms(500);

        
    }
}
