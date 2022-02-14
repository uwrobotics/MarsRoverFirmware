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

void CANDriverModule::rxPostman(void) {
  interface.rxPostman();
}

bool CANDriverModule::sendOneShotMessage(CANMsg &msg, Kernel::Clock::duration_u32 timeout) {
  return interface.sendOneShotMessage(msg, timeout);
}

bool CANDriverModule::setTXSignalValue(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName,
                                       HWBRIDGE::CANSignalValue_t signalValue) {
    return interface.setTXSignalValue(msgID, signalName, signalValue);
}

bool CANDriverModule::getRXSignalValue(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName, HWBRIDGE::CANSignalValue_t &signalValue) {
  return interface.getRXSignalValue(msgID, signalName, signalValue);  
}

bool CANDriverModule::setFilter(HWBRIDGE::CANFILTER filter, CANFormat format = CANStandard,
                                uint16_t mask = HWBRIDGE::ROVER_CANID_FILTER_MASK, int handle = 0) {
  return interface.setFilter(filter, format, mask, handle);
}

uint32_t CANDriverModule::getNumStreamedMsgsReceived(void) {
  return interface.getNumStreamedMsgsReceived();
}

uint32_t CANDriverModule::getNumOneShotMsgsReceived(void) {
  return interface.getNumOneShotMsgsReceived();
}

uint32_t CANDriverModule::getNumStreamedMsgsSent(void) {
  return interface.getNumStreamedMsgsSent();
}

uint32_t CANDriverModule::getNumOneShotMsgsSent(void) {
  return interface.getNumOneShotMsgsSent();
}

uint16_t CANDriverModule::getNumCANRXFaults(void) {
  return interface.getNumCANRXFaults();
}

uint16_t CANDriverModule::getNumCANTXFaults(void) {
  return interface.getNumCANTXFaults();
}


