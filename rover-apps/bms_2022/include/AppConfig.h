#pragma once

#include <vector>

#include "Module.h"
#include "BMSMonitoring.h"

BMSMonitoring BMS_Monitoring;

std::vector<Module*> gModules = {
    &BMS_Monitoring,
};
