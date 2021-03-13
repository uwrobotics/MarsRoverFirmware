#pragma once

#include "LimServo.h"

namespace Roll {

/*
• Roll: SG90 (https://datasheetspdf.com/pdf/791970/TowerPro/SG90/1)
*/

namespace Internal {
constexpr auto RANGE     = 180;
constexpr auto MAX_PULSE = 2ms, MIN_PULSE = 1ms;
}  // namespace Internal

static Actuator::LimServo rollServo(SRVO_PWM_ROLL, Internal::RANGE, Internal::MAX_PULSE, Internal::MIN_PULSE);
}  // namespace Roll
