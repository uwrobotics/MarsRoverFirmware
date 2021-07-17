#include "AdafruitSTEMMAModule.h"
#include <ratio>
#include "Module.h"
#include "mbed.h"
#include "AdafruitSTEMMA.h"

AdafruitSTEMMAModule::AdafruitSTEMMAModule() : moistureSensor(TEMP_MOIST_I2C_ADA, TEMP_MOIST_I2C_SCL) {
    static int numFails = 0;
    bool resetStatus = false;

    while (!resetStatus && numFails < 10) {
        resetStatus = moistureSensor.reset();
    }
    // if still error then log
}

void AdafruitSTEMMAModule::periodic_100ms(void) {
    static int numFails = 0;
    bool updateStatus = false;

    while (!updateStatus && numFails < 10) {
        updateStatus = moistureSensor.update();
    }
    // if still error then log
}