#pragma once

#include <vector>

#include "Module.h"
#include "ControllerModule.h"

HWBRIDGE::CANSignalValue_t panSetPoint;
HWBRIDGE::CANSignalValue_t pitchPosition;

ControllerModule gimbal_controller(CANInterface &can);

std::vector<Module*> gModules = {
    &gimbal_controller;
};
