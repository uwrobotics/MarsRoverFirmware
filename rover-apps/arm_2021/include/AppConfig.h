#pragma once

#include <vector>

#include "Module.h"
#include "WatchdogModule.h"
#include "CANDriverModule.h"
#include "CANConfig.h"
#include "hw_bridge.h"
#include "CANInterface.h"

WatchdogModule arm_watchdog;

std::vector<Module*> gModules = {
    // put modules here
    &arm_watchdog,
};
