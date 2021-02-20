#pragma once

#include "LimServo.h"

namespace Cover {

namespace Internal {
constexpr auto RANGE     = 90;  // 90 DEGREES - @Mathieu
constexpr auto MAX_PULSE = 2.2ms, MIN_PULSE = 0.8ms;
}  // namespace Internal

static Actuator::LimServo servo(SRVO_PWM_LID, Internal::RANGE, Internal::MAX_PULSE, Internal::MIN_PULSE);

}  // namespace Cover
