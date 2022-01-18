#pragma once

#include <vector>

#include "Module.h"
#include "WatchdogModule.h"

WatchdogModule science_watchdog;

std::vector<Module*> gModules = {
    &science_watchdog,
};
