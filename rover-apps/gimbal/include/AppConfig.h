#pragma once

#include <vector>

#include "Module.h"
#include "WatchdogModule.h"

WatchdogModule gimbal_watchdog;
 
std::vector<Module*> gModules = {
    // put modules here
    &gimbal_watchdog,
};
