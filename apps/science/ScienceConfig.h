#pragma once

#include "mbed.h"
#include "../targets/science/PinNames.h"
#include "Encoder.h"
#include "ActuatorController.h"
#include "Servo.h"

namespace ScienceConfig {

    /* Encoder Config */

    static Encoder::t_encoderConfig diggerLiftEncoderConfig = {
        .pin_ChannelA = ENC_DCI_A,
        .pin_ChannelB = ENC_DCI_B,

        .degreesPerUnit = 360.0/610.74
    };

    static Encoder::t_encoderConfig indexerEncoderConfig = {
        
    };

    /* Servo Config */
    static Servo::SERVO_TYPE coverServoType = Servo::LIM_SERVO;
    static float coverServoRange = 120.0;
    static float coverServoMaxPulse = 2.2;
    static float coverServoMinPulse = 0.8;

    static Servo::SERVO_TYPE diggerServoType = Servo::LIM_SERVO;
    static float diggerServoRange = 120.0;
    static float diggerServoMaxPulse = 2.2;
    static float diggerServoMinPulse = 0.8;

    /* Rotary Actuator Config */

    static ActuatorController::t_actuatorConfig diggerLiftActuatorConfig = {
        .defaultControlMode = position,
        .positionPID = {
            .P = 1.0, 
            .I = 0.0,
            .D = 0.0, 
            .bias = 0.0,
            .deadZoneError = 0.1
        }
    };

    static ActuatorController::t_actuatorConfig indexerActuatorConfig;

} //namespace ScienceConfig