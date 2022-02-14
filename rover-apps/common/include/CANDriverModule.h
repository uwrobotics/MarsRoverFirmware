#pragma once

#include "CANInterface.h"
#include "Module.h"
#include "mbed.h"
#include "CANBus.h"
#include "CANMsg.h"
#include "hw_bridge.h"
#include "mbed.h"

class CANDriverModule final : public Module {
 public:
  CANDriverModule(const CANInterface::Config &config);
  void periodic_1s(void) override;

  void periodic_10s(void) override;
  void periodic_100ms(void) override;
  void periodic_10ms(void) override;
  void periodic_1ms(void) override;
  void rxPostman(void);
  bool sendOneShotMessage(CANMsg &msg, Kernel::Clock::duration_u32 timeout);

  // Update a TX CAN signal
  bool setTXSignalValue(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName, HWBRIDGE::CANSignalValue_t signalValue);

  // Read a RX CAN signal
  bool getRXSignalValue(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName, HWBRIDGE::CANSignalValue_t &signalValue);

  // Set CAN bus hw filter
  bool setFilter(HWBRIDGE::CANFILTER filter, CANFormat format = CANStandard,
                 uint16_t mask = HWBRIDGE::ROVER_CANID_FILTER_MASK, int handle = 0);

  // For diagnostic purposes
  uint32_t getNumStreamedMsgsReceived(void);
  uint32_t getNumOneShotMsgsReceived(void);
  uint32_t getNumStreamedMsgsSent(void);
  uint32_t getNumOneShotMsgsSent(void);

  uint16_t getNumCANRXFaults(void);
  uint16_t getNumCANTXFaults(void);

 private:
  CANInterface interface;
};
