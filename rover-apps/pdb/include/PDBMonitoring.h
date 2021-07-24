#pragma once

#include "Module.h"
#include "mbed.h"

/*This PDB module is for load, rail and temperature monitoring.*/

class PDBMonitoring final : public Module {
 public:
  /* Sets the Load DIAG_EN pins */
  PDBMonitoring();

  void periodic_1s(void) override;
  void periodic_10s(void) override {}
  void periodic_100ms(void) override {}
  void periodic_10ms(void) override {}
  void periodic_1ms(void) override;

 private:
  void load_monitoring();
  void rail_monitoring();
  void temperature_monitoring();

  static const float PDB_VBAT_RAIL_NOMINAL_VOLTAGE;
  static const float PDB_VBAT_RAIL_MIN_THRESHOLD;
  static const float PDB_VBAT_RAIL_MAX_THRESHOLD;

  static const float PDB_24V_RAIL_NOMINAL_VOLTAGE;
  static const float PDB_24V_RAIL_MIN_THRESHOLD;
  static const float PDB_24V_RAIL_MAX_THRESHOLD;

  static const float PDB_17V_RAIL_NOMINAL_VOLTAGE;
  static const float PDB_17V_RAIL_MIN_THRESHOLD;
  static const float PDB_17V_RAIL_MAX_THRESHOLD;

  static const float PDB_5V_RAIL_NOMINAL_VOLTAGE;
  static const float PDB_5V_RAIL_MIN_THRESHOLD;
  static const float PDB_5V_RAIL_MAX_THRESHOLD;

  static const float PDB_TEMPERATURE_MIN_THRESHOLD;
  static const float PDB_TEMPERATURE_MAX_THRESHOLD;

  static const bool PDB_5V_LOAD1_DIAG_EN;
  static const bool PDB_5V_LOAD2_DIAG_EN;
  static const bool PDB_5V_LOAD3_DIAG_EN;
  static const bool PDB_5V_LOAD4_DIAG_EN;
  static const bool PDB_5V_LOAD5_DIAG_EN;
  static const bool PDB_17V_LOAD_DIAG_EN;
};