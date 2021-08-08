#pragma once

#include <vector>

#include "AdafruitSTEMMAModule.h"
#include "Module.h"

AdafruitSTEMMAModule moistureSensorModule{TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL};

std::vector<Module*> gModules = {
    // put modules here
    &moistureSensorModule,
};
