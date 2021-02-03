#pragma once

#include "ActuatorController.h"
#include "Encoder.h"
#include "PinNames.h"
#include "Servo.h"
#include "mbed.h"

constexpr uint8_t FLAG_DISABLE_LIMIT_SWITCH_CHECKS = (1U << 0);
constexpr uint8_t FLAG_DISABLE_ANGLE_BOUNDS_CHECKS = (1U << 1);
constexpr uint8_t FLAG_DISABLE_FEEDBACK            = (1U << 2);

namespace ScienceConfig {

/*** ENCODER CONFIG ***/
static Encoder::t_encoderConfig elevatorEncoderConfig = {
    .pin_PWM = ENC_SCI_IN, .degreesPerUnit = 360, .zeroOffset_Degrees = 0.0};

static Encoder::t_encoderConfig centrifugeEncoderConfig{.pin_ChannelA = ENC_DCI_A,
                                                        .pin_ChannelB = ENC_DCI_B,

                                                        .degreesPerUnit     = 1.0,
                                                        .zeroOffset_Degrees = 0.0};

/* Servo Config */
static float coverServoRange                           = 120.0;
static std::chrono::duration<float> coverServoMaxPulse = 2.2ms;
static std::chrono::duration<float> coverServoMinPulse = 0.8ms;

static float diggerServoRange                           = 120.0;
static std::chrono::duration<float> diggerServoMaxPulse = 2.2ms;
static std::chrono::duration<float> diggerServoMinPulse = 0.8ms;

static ActuatorController::t_actuatorConfig diggerLiftActuatorConfig = {
    .defaultControlMode = ActuatorController::t_actuatorControlMode::position,
    .positionPID        = {.P = 1.0, .I = 0.0, .D = 0.0, .bias = 0.0, .deadZoneError = 0.1}};

static ActuatorController::t_actuatorConfig indexerActuatorConfig;

}  // namespace ScienceConfig
