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
}