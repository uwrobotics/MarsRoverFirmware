#pragma once

#include "LimServo.h"

// Servo Model: TowerPro SG90

namespace Roll {

namespace Internal {
constexpr auto range    = 180;
constexpr auto maxPulse = 2ms, minPulse = 1ms;
}  // namespace Internal

static Actuator::LimServo rollServo(SRVO_PWM_ROLL, Internal::range, Internal::maxPulse, Internal::minPulse);
}  // namespace Roll
