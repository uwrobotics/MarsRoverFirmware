#pragma once

#include "Module.h"
#include "PDB_config.h"
#include "mbed.h"

class PDB_Module final : public Module {
 public:
  /* Sets the Load DIAG_EN pins */
  PDB_Module();

  void periodic_1s(void) override;
  void periodic_10s(void) override {}
  void periodic_100ms(void) override {}
  void periodic_10ms(void) override {}
  void periodic_1ms(void) override;

 private:
  void load_monitoring();
  void rail_monitoring();
  void temperature_monitoring();
  int rail_counter;
};