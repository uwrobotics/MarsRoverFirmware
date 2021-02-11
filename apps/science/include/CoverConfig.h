#pragma once

#include "LimServo.h"

namespace Cover {

namespace Internal {
constexpr auto range    = 120;
constexpr auto maxPulse = 2.2ms, minPulse = 0.8ms;
}  // namespace Internal

static Actuator::LimServo servo(SRVO_PWM_1, Internal::range, Internal::maxPulse, Internal::minPulse);

}  // namespace Cover