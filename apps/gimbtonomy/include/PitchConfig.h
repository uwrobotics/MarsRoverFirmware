#pragma once

#include "LimServo.h"

namespace Pitch {

namespace Internal {
constexpr auto RANGE    = 180;  // Orson TODO: confirm these 3 values with EE and ME
constexpr auto MAXPULSE = 2.1ms, MINPULSE = 0.9ms;
}  // namespace Internal

static Actuator::LimServo pitchServo(SRVO_PWM_HS, Internal::RANGE, Internal::MAXPULSE, Internal::MINPULSE);
}  // namespace Pitch