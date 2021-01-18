#pragma once

#include "MoistureSensor.h"
#include "hw_bridge.h"

namespace MoistureSensorConfig {

static MoistureSensor::Config moistureSensorConfig = {.sda = TEMP_MOIST_I2C_SDA, .scl = TEMP_MOIST_I2C_SCL};
}
