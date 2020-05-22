#pragma once

#include "Encoder.h"
#include "ActuatorController.h"

constexpr uint8_t FLAG_DISABLE_LIMIT_SWITCH_CHECKS = (1U << 0);
constexpr uint8_t FLAG_DISABLE_ANGLE_BOUNDS_CHECKS = (1U << 1);
constexpr uint8_t FLAG_DISABLE_FEEDBACK = (1U << 2);

namespace ScienceConfig
{

    /*** ENCODER CONFIG ***/
    static Encoder::t_encoderConfig elevatorEncoderConfig = {
        .pin_PWM = ENC_SCI_IN,
        .degreesPerUnit = 360,
        .zeroOffset_Degrees = 0.0};

    static Encoder::t_encoderConfig centrifugeEncoderConfig{
        .pin_ChannelA = ENC_DCI_A,
        .pin_ChannelB = ENC_DCI_B,

        .degreesPerUnit = 1.0,
        .zeroOffset_Degrees = 0.0};
} // namespace ScienceConfig
