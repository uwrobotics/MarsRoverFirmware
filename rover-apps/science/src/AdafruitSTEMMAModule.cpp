#include "AdafruitSTEMMAModule.h"
#include "Module.h"
#include "mbed.h"
#include "AdafruitSTEMMA.h"

AdafruitSTEMMAModule::AdafruitSTEMMAModule() {
    moistureSensorModule = new AdafruitSTEMMA(TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL);
    bool resetStatus = moistureSensor -> reset();
}

void AdafruitSTEMMAModule::periodic_100ms(void) {
    bool updateStatus = moistureSensor -> update();
}