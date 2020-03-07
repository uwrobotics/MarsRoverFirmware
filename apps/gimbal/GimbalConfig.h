#pragma once

#include "Encoder.h"
#include "ActuatorController.h"
#include "mbed.h"

namespace GimbConfig {

    /*** ENCODER CONFIG ***/

    static Encoder::t_encoderConfig panEncoderConfig = {
        .pin_PWM = ENC_PWM_GIMB,
        .degreesPerUnit = 360.0
    };

    /*** MOTOR CONFIG***/

    static Motor::t_motorConfig panMotorConfig = {
        .pwmPin = SRVO_PWM_CR,
        .dirPin = NC,
        .inverted = false,
        .freqInHz = 0,
        .limit = 0.0f
    };

    static Motor::t_motorConfig tiltMotorConfig = {
        .pwmPin = SRVO_PWM_MG,
        .dirPin = NC,
        .inverted = false,
        .freqInHz = 0,
        .limit = 0.0f
    };

    /*** ACTUATOR CONFIG ***/
    static ActuatorController::t_actuatorControlMode panActuatorControlMode = ActuatorController::velocity;

    static ActuatorController::t_actuatorConfig panActuatorConfig = {
        panActuatorControlMode,
        .minAngle_Degrees = -360.0,
        .maxAngle_Degrees = 360.0,

        .velocityPID = {
            .P = 1.0, 
            .I = 0.0,
            .D = 0.0, 
            .bias = 0.0,
            .deadZoneError = 0.1,
        }, 

        .positionPID = {
            .P = 1.0, 
            .I = 0.0,
            .D = 0.0,
            .bias = 0.0,
            .deadZoneError = 0.1
        }
    };

    static ActuatorController::t_actuatorControlMode tiltActuatorControlMode = ActuatorController::position;
    static ActuatorController::t_actuatorConfig tiltActuatorConfig = {
        tiltActuatorControlMode,
        .minAngle_Degrees = -90,
        .maxAngle_Degrees = 90,
        .velocityPID = {
            .P = 1.0, 
            .I = 0.0,
            .D = 0.0, 
            .bias = 0.0,
            .deadZoneError = 0.1,
        }, 

        .positionPID = {
            .P = 1.0, 
            .I = 0.0,
            .D = 0.0,
            .bias = 0.0,
            .deadZoneError = 0.1
        }        
    };
}
