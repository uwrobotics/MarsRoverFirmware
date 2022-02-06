#pragma once

#include <vector>

#include "BMSMonitoring.h"
#include "Module.h"

BMSMonitoring BMS_Monitoring;

std::vector<Module*> gModules = {
    &BMS_Monitoring,
};
