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

 private:
  CANInterface interface;
};
