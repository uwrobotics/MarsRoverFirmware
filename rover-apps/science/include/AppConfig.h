#pragma once

#include <vector>

#include "Module.h"
#include "AdafruitSTEMMAModule.h"

AdafruitSTEMMAModule moistureSensorModule;

std::vector<Module*> gModules = {
    // put modules here
    &moistureSensorModule,
};
