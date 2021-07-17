#pragma once

#include <vector>

#include "Module.h"
#include "AdafruitSTEMMAModule.h"

AdafruitSTEMMAModule moistureSensorModule = new moistureSensorModule(TEMP_MOIST_I2C_ADA, TEMP_MOIST_I2C_SCL);

std::vector<Module*> gModules = {
    // put modules here
    &moistureSensorModule,
};
