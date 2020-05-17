#pragma once

#include "ActuatorController.h"
#include "Encoder.h"

constexpr uint8_t FLAG_DISABLE_LIMIT_SWITCH_CHECKS = (1U << 0);
constexpr uint8_t FLAG_DISABLE_ANGLE_BOUNDS_CHECKS = (1U << 1);
constexpr uint8_t FLAG_DISABLE_FEEDBACK            = (1U << 2);

namespace ArmConfig {

/*** ENCODER CONFIG ***/

static Encoder::t_encoderConfig turnTableEncoderConfig = {ENC_PWM_TRNTBL,      NC, NC, NC, NC, NC, NC, 360.0, 0,
                                                          Encoder::x2_encoding};

static Encoder::t_encoderConfig shoulderEncoderConfig = {ENC_PWM_SHLDR,       NC, NC, NC, NC, NC, NC, 360.0, 0,
                                                         Encoder::x2_encoding};

static Encoder::t_encoderConfig elbowEncoderConfig = {ENC_PWM_ELBW, NC, NC,    NC, NC,
                                                      NC,           NC, 360.0, 0,  Encoder::x2_encoding};

static Encoder::t_encoderConfig wristLeftEncoderConfig = {
    NC, NC, NC, NC, ENC_QUAD_WRST_LHS_A, ENC_QUAD_WRST_LHS_A, NC, 1.0, 0.0, Encoder::x2_encoding};

static Encoder::t_encoderConfig wristRightEncoderConfig = {
    NC, NC, NC, NC, ENC_QUAD_WRST_RHS_A, ENC_QUAD_WRST_RHS_B, NC, 1.0, 0.0, Encoder::x2_encoding};

static Encoder::t_encoderConfig clawEncoderConfig = {
    NC, NC, NC, NC, ENC_QUAD_CLAW_A, ENC_QUAD_CLAW_B, NC, 1.0, 0.0, Encoder::x2_encoding};

/*** ROTARY ACTUATOR CONFIG ***/

static ActuatorController::t_actuatorConfig turnTableActuatorConfig = {
    ActuatorController::motorPower, -1.0, +1.0, -10.0, +10.0, -90, +90, {1.0, 0.0, 0.0, 0.0, 0.1},
    {1.0, 0.0, 0.0, 0.0, 0.1},      3.0};

static ActuatorController::t_actuatorConfig shoulderActuatorConfig = {
    ActuatorController::motorPower, -1.0, +1.0, -10.0, +10.0, -90, +90, {1.0, 0.0, 0.0, 0.0, 0.1},
    {1.0, 0.0, 0.0, 0.0, 0.1},      3.0};

static ActuatorController::t_actuatorConfig elbowActuatorConfig = {
    ActuatorController::motorPower, -1.0, +1.0, -10.0, +10.0, -90, +90, {1.0, 0.0, 0.0, 0.0, 0.1},
    {1.0, 0.0, 0.0, 0.0, 0.1},      3.0};

static ActuatorController::t_actuatorConfig wristLeftActuatorConfig = {
    ActuatorController::motorPower, -1.0, +1.0, -10.0, +10.0, -90, +90, {1.0, 0.0, 0.0, 0.0, 0.1},
    {1.0, 0.0, 0.0, 0.0, 0.1},      3.0};

static ActuatorController::t_actuatorConfig wristRightActuatorConfig = {
    ActuatorController::motorPower, -1.0, +1.0, -10.0, +10.0, -90, +90, {1.0, 0.0, 0.0, 0.0, 0.1},
    {1.0, 0.0, 0.0, 0.0, 0.1},      3.0};

static ActuatorController::t_actuatorConfig clawActuatorConfig = {
    ActuatorController::motorPower, -1.0, +1.0, -10.0, +10.0, -90, +90, {1.0, 0.0, 0.0, 0.0, 0.1},
    {1.0, 0.0, 0.0, 0.0, 0.1},      3.0};

}  // namespace ArmConfig
