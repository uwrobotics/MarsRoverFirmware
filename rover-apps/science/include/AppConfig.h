#pragma once

#include <vector>

#include "Module.h"
#include "WatchdogModule.h"

std::chrono::milliseconds countdown_ms = 1000ms; /*Test value*/

Module::WatchdogModule science_watchdog(countdown_ms);

std::vector<Module*> gModules = {
    // put modules here
    &science_watchdog,
};
