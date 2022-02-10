#include "CANDriverModule.h"
#include "CANInterface.h"
#include "Module.h"
#include "mbed.h"

// Using initializer lists to construct CANInterface object will attach ISRs and other required tasks
CANDriverModule::CANDriverModule(const CANInterface::Config &config) : interface(config) {
  // Nothing should be required here
}

void CANDriverModule::periodic_1ms(void) {

    // These functions need to be called at 1 kHz, or every 1ms
    interface.rxClientPeriodic();
}

void CANDriverModule::periodic_10ms(void) {

    // These functions need to be called every 100 Hz (10ms)
    interface.txProcessorPeriodic();
}