#include "CANModule.h"

#include "Module.h"
#include "mbed.h"

// Using initializer lists to construct CANInterface object will attach ISRs and other required tasks
CANModule::CANModule(const Config &config) : interface(config) {
    // Nothing should be required here
}

void CANModule::periodic_1ms(void) {

    // These functions need to be called at 1 kHz, or every 1ms
    interface.rxClient();
}

void CANModule::periodic_10ms(void) {

    // These functions need to be called every 100 Hz (10ms)
    interface.txProcessor();
}