#pragma once

#include <vector>

#include "Module.h"
#include "WatchdogModule.h"

WatchdogModule arm_watchdog;

std::vector<Module*> gModules = {
    &arm_watchdog,
};
