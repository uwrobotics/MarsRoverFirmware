#pragma once

#include "LimServo.h"

namespace Digger {

namespace Internal {
constexpr auto range    = 120; // Felix TODO: Confirm these values with EE and ME
constexpr auto maxPulse = 2.2ms, minPulse = 0.8ms;
}  // namespace Internal

static Actuator::LimServo servo(SRVO_PWM_2, Internal::range, Internal::maxPulse, Internal::minPulse);

}  // namespace Digger
