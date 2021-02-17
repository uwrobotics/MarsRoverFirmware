#pragma once

#include "LimServo.h"

namespace Pitch {

namespace Internal {
constexpr auto range    = 195;
constexpr auto maxPulse = 2.5ms, minPulse = 0.5ms;
}  // namespace Internal

static Actuator::LimServo pitchServo(SRVO_PWM_PITCH, Internal::range, Internal::maxPulse, Internal::minPulse);
}  // namespace Pitch
