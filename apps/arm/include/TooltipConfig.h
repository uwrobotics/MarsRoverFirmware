#pragma once

#include "LimServo.h"

namespace Tooltip {
static Actuator::LimServo clawTooltipServo(SRVO_PWM_CLAW, 180.0, 2ms, 1ms);
}  // namespace Tooltip
