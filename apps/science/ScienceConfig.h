#pragma once

#include "mbed.h"
#include "../targets/science/PinNames.h"
#include "Encoder.h"
#include "ActuatorController.h"

namespace ScienceConfig {

    /* Encoder Config */

    static Encoder::t_encoderConfig diggerLiftEncoderConfig = {
        .pin_ChannelA = ENC_DCI_A,
        .pin_ChannelB = ENC_DCI_B,

        .degreesPerUnit = 360.0/610.74
    };

    static Encoder::t_encoderConfig indexerEncoderConfig = {
        
    };

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