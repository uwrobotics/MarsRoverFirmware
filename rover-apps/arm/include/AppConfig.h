#pragma once

#include <vector>

#include "Module.h"
#include "ControllerModule.h"

ControllerModule arm_controller;

std::vector<Module*> gModules = {
    // put modules here
    &arm_controller;
};
