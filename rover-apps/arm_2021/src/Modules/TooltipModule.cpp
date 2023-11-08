#pragma once

#include "TooltipModule.h"

ClawModule::ClawModule()
    : toolTipPosition(0), clawTooltipServo(Actuator::LimServo(SRVO_PWM_TOOLTIP, 180.0, 2ms, 1ms)) {}

void periodic_1ms(CANInterface& can) override {
  can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_TOOL_TIP_POSITION,
                       toolTipPosition);
  toolTipPosition = RAD_TO_DEG(toolTipPosition);

  Tooltip::clawTooltipServo.setValue(static_cast<float>(toolTipPosition));
}
