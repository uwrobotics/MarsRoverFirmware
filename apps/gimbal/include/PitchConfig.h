#pragma once

#include "LimServo.h"

namespace Pitch {

/*
• Pitch: HS-422 (https://www.robotshop.com/en/hitec-hs-422-servo-motor.html)
*/

namespace Internal {
constexpr auto RANGE     = 195;
constexpr auto MAX_PULSE = 2.5ms, MIN_PULSE = 0.5ms;
}  // namespace Internal

static Actuator::LimServo pitchServo(SRVO_PWM_PITCH, Internal::RANGE, Internal::MAX_PULSE, Internal::MIN_PULSE);
}  // namespace Pitch
