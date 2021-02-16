#pragma once

#include "LimServo.h"

namespace Cover {

namespace Internal {
constexpr auto RANGE     = 120;  // Felix TODO: Confirm these values with EE and ME
constexpr auto MAX_PULSE = 2.2ms, MIN_PULSE = 0.8ms;
}  // namespace Internal

static Actuator::LimServo servo(SRVO_PWM_LID, Internal::RANGE, Internal::MAX_PULSE, Internal::MIN_PULSE);

}  // namespace Cover
