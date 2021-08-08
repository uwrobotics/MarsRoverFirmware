#pragma once

#include "AdafruitSTEMMA.h"
#include "Module.h"
#include "mbed.h"

class AdafruitSTEMMAModule final : public Module {
 public:
  AdafruitSTEMMAModule(PinName sda, PinName scl);

  void periodic_1s(void) override;
  void periodic_10s(void) override;
  void periodic_100ms(void) override;
  void periodic_10ms(void) override;
  void periodic_1ms(void) override;

 private:
  Sensor::AdafruitSTEMMA moistureSensor;
};
