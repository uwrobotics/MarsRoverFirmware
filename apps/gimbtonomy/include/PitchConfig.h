#pragma once

#include "LimServo.h"

namespace Pitch {

namespace Internal {
constexpr auto range    = 180;  // Orson TODO: confirm these 3 values with EE and ME
constexpr auto maxPulse = 2.1ms, minPulse = 0.9ms;
}  // namespace Internal

static Actuator::LimServo pitchServo(SRVO_PWM_HS, Internal::range, Internal::maxPulse, Internal::minPulse);
}  // namespace Pitch