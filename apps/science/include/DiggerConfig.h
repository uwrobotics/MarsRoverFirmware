#pragma once

#include "LimServo.h"

namespace Digger {

namespace Internal {
constexpr auto RANGE     = 120;  // Felix TODO: Confirm these values with EE and ME
constexpr auto MAX_PULSE = 2.2ms, MIN_PULSE = 0.8ms;
}  // namespace Internal

static Actuator::LimServo servo(SRVO_PWM_SHVL, Internal::RANGE, Internal::MAX_PULSE, Internal::MIN_PULSE);

}  // namespace Digger
