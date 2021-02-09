#pragma once

#include "LimServo.h"

namespace Roll {

namespace Internal {
constexpr auto range    = 180;
constexpr auto maxPulse = 2ms, minPulse = 1ms;
}  // namespace Internal

/* @TODO: electrical hasn't choose a pin for this servo yet, I'm just using a random free pin for this for now*/
static Actuator::LimServo rollServo(SRVO_PWM_SG, Internal::range, Internal::maxPulse, Internal::minPulse);
}  // namespace Roll