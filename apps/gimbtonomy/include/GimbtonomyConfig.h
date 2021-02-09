#pragma once

#include "ActuatorController.h"
#include "Encoder.h"
/*
namespace GimbtonomyConfig {

static Encoder::t_encoderConfig panEncoderConfig{
    .pin_PWM = ENC_PWM_GIMB, .degreesPerUnit = 360.0, .zeroOffset_Degrees = 0.0};

static ActuatorController::t_actuatorConfig panServoActuatorConfig = {
    .defaultControlMode        = ActuatorController::position,
    .minVelocity_DegreesPerSec = -228.0,
    .maxVelocity_DegreesPerSec = +228.0,
    .velocityPID               = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1},
    .positionPID               = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1}};

}  // namespace GimbtonomyConfig
*/