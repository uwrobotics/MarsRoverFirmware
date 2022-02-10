#pragma once

#include <vector>

#include "Module.h"
#include "WatchdogModule.h"
#include "CANDriverModule.h"

WatchdogModule science_watchdog;

std::vector<Module*> gModules = {
    // put modules here
    &science_watchdog,
};
