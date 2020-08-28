#pragma once

#include "ActuatorController.h"
#include "Encoder.h"

constexpr unsigned uint8_t FLAG_DISABLE_LIMIT_SWITCH_CHECKS(1U << 0);
constexpr unsigned uint8_t FLAG_DISABLE_ANGLE_BOUNDS_CHECKS(1U << 1);
constexpr unsigned uint8_t FLAG_DISABLE_FEEDBACK(1U << 2);

namespace ArmConfig {

/*** ENCODER CONFIG ***/

static Encoder::t_encoderConfig turnTableEncoderConfig = {
    .pin_PWM = ENC_PWM_TRNTBL, .degreesPerUnit = 360, .zeroOffset_Degrees = 0.0};

static Encoder::t_encoderConfig shoulderEncoderConfig = {
    .pin_PWM = ENC_PWM_TRNTBL, .degreesPerUnit = 360, .zeroOffset_Degrees = 0.0};

static Encoder::t_encoderConfig elbowEncoderConfig = {
    .pin_PWM = ENC_PWM_TRNTBL, .degreesPerUnit = 360, .zeroOffset_Degrees = 0.0};

static Encoder::t_encoderConfig wristLeftEncoderConfig = {.pin_ChannelA = ENC_QUAD_WRST_LHS_A,
                                                          .pin_ChannelB = ENC_QUAD_WRST_LHS_A,

                                                          .degreesPerUnit     = 1.0,
                                                          .zeroOffset_Degrees = 0.0};

static Encoder::t_encoderConfig wristRightEncoderConfig = {.pin_ChannelA = ENC_QUAD_WRST_RHS_A,
                                                           .pin_ChannelB = ENC_QUAD_WRST_RHS_B,

                                                           .degreesPerUnit     = 1.0,
                                                           .zeroOffset_Degrees = 0.0};

static Encoder::t_encoderConfig clawEncoderConfig = {.pin_ChannelA = ENC_QUAD_CLAW_A,
                                                     .pin_ChannelB = ENC_QUAD_CLAW_B,

                                                     .degreesPerUnit     = 1.0,
                                                     .zeroOffset_Degrees = 0.0};

/*** ROTARY ACTUATOR CONFIG ***/

static ActuatorController::t_actuatorConfig turnTableActuatorConfig = {
    .velocityPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1},
    .positionPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1}};

static ActuatorController::t_actuatorConfig shoulderActuatorConfig = {
    .velocityPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1},
    .positionPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1}};

static ActuatorController::t_actuatorConfig elbowActuatorConfig = {
    .velocityPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1},
    .positionPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1}};

static ActuatorController::t_actuatorConfig wristLeftActuatorConfig = {
    .velocityPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1},
    .positionPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1}};

static ActuatorController::t_actuatorConfig wristRightActuatorConfig = {
    .velocityPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1},
    .positionPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1}};

static ActuatorController::t_actuatorConfig clawActuatorConfig = {
    .velocityPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1},
    .positionPID = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1}};

}  // namespace ArmConfig