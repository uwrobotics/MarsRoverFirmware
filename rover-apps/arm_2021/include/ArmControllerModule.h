#pragma once

#include "CANConfig.h"
#include "Module.h"
#include "mbed.h"

class ArmControllerModule final : public Module {
 public:
  ArmControllerModule();

  // Threads
  void periodic_1s(void) override;
  void periodic_10s(void) override;
  void periodic_100ms(void) override;
  void periodic_10ms(void) override;
  void periodic_1ms(void) override;

  // Member Variables
  CANInterface can(CANConfig::config);

  // Member Functions
  static mbed_error_status_t armSetControlMode(void);
  static mbed_error_status_t armSetJointPIDParams(void);
  static mbed_error_status_t armSetSafetyCheck(void);
  static mbed_error_status_t commonSwitchCANBus(void);
  static void sendACK(HWBRIDGE::ARM_ACK_VALUES ackValue);

  static inline float DEG_TO_RAD(float deg);
  static inline float RAD_TO_DEG(float rad) {
   private:
  }
