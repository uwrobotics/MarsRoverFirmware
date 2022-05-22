#pragma once

#include "Module.h"
#include "mbed.h"

class ClawModule final : public Module {
 public:
  void periodic_1s(void) override;
  void periodic_10s(void) override;
  void periodic_100ms(void) override;
  void periodic_10ms(void) override;
  void periodic_1ms(void) override;

 private:
 
}