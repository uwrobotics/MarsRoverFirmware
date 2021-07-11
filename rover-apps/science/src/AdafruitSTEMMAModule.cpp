#include "AdafruitSTEMMAModule.h"
#include "Module.h"
#include "mbed.h"
#include "AdafruitSTEMMA.h"

AdafruitSTEMMAModule::AdafruitSTEMMAModule() {
    bool resetStatus = moistureSensor.reset();
}

void AdafruitSTEMMAModule::periodic_100ms(void) {
    bool updateStatus = moistureSensor.update();
}