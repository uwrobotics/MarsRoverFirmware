#pragma once

#include <vector>

#include "Module.h"
#include "WatchdogModule.h"

WatchdogModule gimbal_watchdog;

std::vector<Module*> gModules = {
    &gimbal_watchdog,
};
