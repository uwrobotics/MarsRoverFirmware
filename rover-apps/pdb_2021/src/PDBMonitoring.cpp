#include "PDBMonitoring.h"

#include <cmath>

#include "Logger.h"

const float PDBMonitoring::PDB_VBAT_RAIL_NOMINAL_VOLTAGE = 24.0;
const float PDBMonitoring::PDB_VBAT_RAIL_MIN_THRESHOLD   = 18.0;
const float PDBMonitoring::PDB_VBAT_RAIL_MAX_THRESHOLD   = 25.2;

const float PDBMonitoring::PDB_24V_RAIL_NOMINAL_VOLTAGE = 24.0;
const float PDBMonitoring::PDB_24V_RAIL_MIN_THRESHOLD   = 22.0;
const float PDBMonitoring::PDB_24V_RAIL_MAX_THRESHOLD   = 26.0;

const float PDBMonitoring::PDB_17V_RAIL_NOMINAL_VOLTAGE = 17.0;
const float PDBMonitoring::PDB_17V_RAIL_MIN_THRESHOLD   = 16.0;
const float PDBMonitoring::PDB_17V_RAIL_MAX_THRESHOLD   = 18.0;

const float PDBMonitoring::PDB_5V_RAIL_NOMINAL_VOLTAGE = 5.0;
const float PDBMonitoring::PDB_5V_RAIL_MIN_THRESHOLD   = 4.8;
const float PDBMonitoring::PDB_5V_RAIL_MAX_THRESHOLD   = 6.0;

const float PDBMonitoring::PDB_TEMPERATURE_MIN_THRESHOLD = 10.0;
const float PDBMonitoring::PDB_TEMPERATURE_MAX_THRESHOLD = 50.0;

const bool PDBMonitoring::PDB_5V_LOAD1_DIAG_EN = 1;
const bool PDBMonitoring::PDB_5V_LOAD2_DIAG_EN = 0;
const bool PDBMonitoring::PDB_5V_LOAD3_DIAG_EN = 0;
const bool PDBMonitoring::PDB_5V_LOAD4_DIAG_EN = 0;
const bool PDBMonitoring::PDB_5V_LOAD5_DIAG_EN = 0;
const bool PDBMonitoring::PDB_17V_LOAD_DIAG_EN = 1;

PDBMonitoring::PDBMonitoring()
    : load1_5V_diag_en(LOAD1_5V_DIAG_EN),
      load1_5V_fault_n(LOAD1_5V_FAULT),
      load2_5V_diag_en(LOAD2_5V_DIAG_EN),
      load2_5V_fault_n(LOAD2_5V_FAULT),
      load3_5V_diag_en(LOAD3_5V_DIAG_EN),
      load3_5V_fault_n(LOAD3_5V_FAULT),
      load4_5V_diag_en(LOAD4_5V_DIAG_EN),
      load4_5V_fault_n(LOAD4_5V_FAULT),
      load5_5V_diag_en(LOAD5_5V_DIAG_EN),
      load5_5V_fault_n(LOAD5_5V_FAULT),
      load_17V_diag_en(LOAD_17V_DIAG_EN),
      load_17V_fault_n(LOAD_17V_FAULT),
      railBattery(RAIL_BATTERY_ANLG_IN),
      rail5V(RAIL_5V_ANLG_IN),
      rail17V(RAIL_17V_ANLG_IN),
      rail24V(RAIL_24V_ANLG_IN),
      rail24V_pgood_n(RAIL_24V_PGOOD_N),
      temperatureADC(TEMPERATURE_ADC_IN) {
  load1_5V_diag_en = PDB_5V_LOAD1_DIAG_EN;
  load2_5V_diag_en = PDB_5V_LOAD2_DIAG_EN;
  load3_5V_diag_en = PDB_5V_LOAD3_DIAG_EN;
  load4_5V_diag_en = PDB_5V_LOAD4_DIAG_EN;
  load5_5V_diag_en = PDB_5V_LOAD5_DIAG_EN;
  load_17V_diag_en = PDB_17V_LOAD_DIAG_EN;
}

/*TODO:Replace the logger statements with CAN logs
 * after CAN Module is ready */

void PDBMonitoring::load_monitoring() {
  if (load1_5V_diag_en && !load1_5V_fault_n) {
    Utility::logger << "Fault on 5V load 1\n";
  }
  if (load2_5V_diag_en && !load2_5V_fault_n) {
    Utility::logger << "Fault on 5V load 2\n";
  }
  if (load3_5V_diag_en && !load3_5V_fault_n) {
    Utility::logger << "Fault on 5V load 3\n";
  }
  if (load4_5V_diag_en && !load4_5V_fault_n) {
    Utility::logger << "Fault on 5V load 4\n";
  }
  if (load5_5V_diag_en && !load5_5V_fault_n) {
    Utility::logger << "Fault on 5V load 5\n";
  }
  if (load_17V_diag_en && !load_17V_fault_n) {
    Utility::logger << "Fault on 17V load\n";
  }
}

void PDBMonitoring::rail_monitoring() {
  float rail_vbat_voltage = railBattery.read_voltage() / 3 * PDB_VBAT_RAIL_NOMINAL_VOLTAGE;
  float rail_24V_voltage  = rail24V.read_voltage() / 3 * PDB_24V_RAIL_NOMINAL_VOLTAGE;
  float rail_17V_voltage  = rail17V.read_voltage() / 3 * PDB_17V_RAIL_NOMINAL_VOLTAGE;
  float rail_5V_voltage   = rail5V.read_voltage() / 3 * PDB_5V_RAIL_NOMINAL_VOLTAGE;

  if (rail_vbat_voltage < PDB_VBAT_RAIL_MIN_THRESHOLD || rail_vbat_voltage > PDB_VBAT_RAIL_MAX_THRESHOLD) {
    Utility::logger << "!!! VBAT RAIL VOLTAGE: %.3fV !!!\n", rail_vbat_voltage;
  }
  if (rail_24V_voltage < PDB_24V_RAIL_MIN_THRESHOLD || rail_24V_voltage > PDB_24V_RAIL_MAX_THRESHOLD) {
    Utility::logger << "!!! 24V RAIL VOLTAGE: %.3fV !!!\n", rail_24V_voltage;
  }
  if (rail_17V_voltage < PDB_17V_RAIL_MIN_THRESHOLD || rail_17V_voltage > PDB_17V_RAIL_MAX_THRESHOLD) {
    Utility::logger << "!!! 17V RAIL VOLTAGE: %.3fV !!!\n", rail_17V_voltage;
  }
  if (rail_5V_voltage < PDB_5V_RAIL_MIN_THRESHOLD || rail_5V_voltage > PDB_5V_RAIL_MAX_THRESHOLD) {
    Utility::logger << "!!! 5V RAIL VOLTAGE: %.3fV !!!\n", rail_5V_voltage;
  }
}

/*https://www.ti.com/lit/ds/symlink/lmt87-q1.pdf?ts=1627158177761&ref_url=https%253A%252F%252Fwww.google.com%252F*/

void PDBMonitoring::temperature_monitoring() {
  float temperature_celsius =
      (13.582 - sqrt(pow(-13.582, 2) + 4 * 0.00433 * (2230.8 - temperatureADC.read_voltage() * 1000))) /
          (2 * (-0.00433)) +
      30;

  if (temperature_celsius < PDB_TEMPERATURE_MIN_THRESHOLD || temperature_celsius > PDB_TEMPERATURE_MAX_THRESHOLD) {
    Utility::logger << "!!! TEMPERATURE FAULT:" << temperature_celsius << "degrees Celsius\n";
  }
}

void PDBMonitoring::periodic_1s() {
  temperature_monitoring();
  load_monitoring();
}

void PDBMonitoring::periodic_1ms() {
  /*Monitoring Period = 500ms*/
  int rail_counter = 0;

  if (rail_counter % 5 == 0) {
    rail_monitoring();
    rail_counter = 0;
  }

  rail_counter++;
}
