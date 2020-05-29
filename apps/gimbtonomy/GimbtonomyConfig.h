#pragma once

#include "ActuatorController.h"
#include "Encoder.h"

namespace GimbtonomyConfig {

static Encoder::t_encoderConfig panEncoderConfig{ENC_PWM_GIMB, NC, NC, NC, NC, NC, NC, 360.0, 0, Encoder::x2_encoding};

static ActuatorController::t_actuatorConfig panServoActuatorConfig = {
    ActuatorController::position, -1.0, +1.0, -264.0, +264.0, -90, +90, {1.0, 0.0, 0.0, 0.0, 0.1},
    {1.0, 0.0, 0.0, 0.0, 0.1},    3.0};

}  // namespace GimbtonomyConfig