#include "PDB_Module.h"

#include <cmath>

#include "CANBus.h"
#include "CANMsg.h"
#include "Logger.h"
#include "PDB_config.h"
#include "hw_bridge.h"

/* Pins configuration for Load Monitoring */
DigitalOut load1_5V_diag_en(LOAD1_5V_DIAG_EN);
DigitalIn load1_5V_fault_n(LOAD1_5V_FAULT);

DigitalOut load2_5V_diag_en(LOAD2_5V_DIAG_EN);
DigitalIn load2_5V_fault_n(LOAD2_5V_FAULT);

DigitalOut load3_5V_diag_en(LOAD3_5V_DIAG_EN);
DigitalIn load3_5V_fault_n(LOAD3_5V_FAULT);

DigitalOut load4_5V_diag_en(LOAD4_5V_DIAG_EN);
DigitalIn load4_5V_fault_n(LOAD4_5V_FAULT);

DigitalOut load5_5V_diag_en(LOAD5_5V_DIAG_EN);
DigitalIn load5_5V_fault_n(LOAD5_5V_FAULT);

DigitalOut load_17V_diag_en(LOAD_17V_DIAG_EN);
DigitalIn load_17V_fault_n(LOAD_17V_FAULT);

/* Pins configuration for Rail Monitoring */
AnalogIn railBattery(RAIL_BATTERY_ANLG_IN);
AnalogIn rail5V(RAIL_5V_ANLG_IN);
AnalogIn rail17V(RAIL_17V_ANLG_IN);
AnalogIn rail24V(RAIL_24V_ANLG_IN);
DigitalIn rail24V_pgood_n(RAIL_24V_PGOOD_N);

PDB_Module::PDB_Module() {
  load1_5V_diag_en = PDB_5V_LOAD1_DIAG_EN;
  load2_5V_diag_en = PDB_5V_LOAD2_DIAG_EN;
  load3_5V_diag_en = PDB_5V_LOAD3_DIAG_EN;
  load4_5V_diag_en = PDB_5V_LOAD4_DIAG_EN;
  load5_5V_diag_en = PDB_5V_LOAD5_DIAG_EN;
  load_17V_diag_en = PDB_17V_LOAD_DIAG_EN;

  rail_counter = 0;
}

void PDB_Module::load_monitoring() {
  if (load1_5V_diag_en && !load1_5V_fault_n) {
    Utility::Logger::printf("Fault on 5V load 1\n");
  }
  if (load2_5V_diag_en && !load2_5V_fault_n) {
    Utility::Logger::printf("Fault on 5V load 2\n");
  }
  if (load3_5V_diag_en && !load3_5V_fault_n) {
    Utility::Logger::printf("Fault on 5V load 3\n");
  }
  if (load4_5V_diag_en && !load4_5V_fault_n) {
    Utility::Logger::printf("Fault on 5V load 4\n");
  }
  if (load5_5V_diag_en && !load5_5V_fault_n) {
    Utility::Logger::printf("Fault on 5V load 5\n");
  }
  if (load_17V_diag_en && !load_17V_fault_n) {
    Utility::Logger::printf("Fault on 17V load\n");
  }
}

void PDB_Module::rail_monitoring() {
  float rail_vbat_voltage = railBattery.read_voltage() / 3 * PDB_VBAT_RAIL_NOMINAL_VOLTAGE;
  float rail_24V_voltage  = rail24V.read_voltage() / 3 * PDB_24V_RAIL_NOMINAL_VOLTAGE;
  float rail_17V_voltage  = rail17V.read_voltage() / 3 * PDB_17V_RAIL_NOMINAL_VOLTAGE;
  float rail_5V_voltage   = rail5V.read_voltage() / 3 * PDB_5V_RAIL_NOMINAL_VOLTAGE;

  if (rail_vbat_voltage < PDB_VBAT_RAIL_MIN_THRESHOLD || rail_vbat_voltage > PDB_VBAT_RAIL_MAX_THRESHOLD) {
    Utility::Logger::printf("!!! VBAT RAIL VOLTAGE: %.3fV !!!\n", rail_vbat_voltage);
  }
  if (rail_24V_voltage < PDB_24V_RAIL_MIN_THRESHOLD || rail_24V_voltage > PDB_24V_RAIL_MAX_THRESHOLD) {
    Utility::Logger::printf("!!! 24V RAIL VOLTAGE: %.3fV !!!\n", rail_24V_voltage);
  }
  if (rail_17V_voltage < PDB_17V_RAIL_MIN_THRESHOLD || rail_17V_voltage > PDB_17V_RAIL_MAX_THRESHOLD) {
    Utility::Logger::printf("!!! 17V RAIL VOLTAGE: %.3fV !!!\n", rail_17V_voltage);
  }
  if (rail_5V_voltage < PDB_5V_RAIL_MIN_THRESHOLD || rail_5V_voltage > PDB_5V_RAIL_MAX_THRESHOLD)) {
      Utility::Logger::printf("!!! 5V RAIL VOLTAGE: %.3fV !!!\n", rail_5V_voltage);
    }
}

void PDB_Module::temperature_monitoring() {
  float temperature_celsius =
      (13.582 - sqrt(pow(-13.582, 2) + 4 * 0.00433 * (2230.8 - temperatureADC.read_voltage() * 1000))) /
          (2 * (-0.00433)) +
      30;

  if (temperature_celsius < PDB_TEMPERATURE_MIN_THRESHOLD || temperature_celsius > PDB_TEMPERATURE_MAX_THRESHOLD)) {
      Utility::Logger::printf("!!! TEMPERATURE FAULT: %.3f degrees Celsius\n", temperature_celsius);
    }
}

void PDB_Module::periodic_1s() {
  temperature_monitoring();
  load_monitoring();
}

void PDB_Module::periodic_1ms() {
  /*Monitoring Period = 500ms*/
  if (rail_counter % 5 == 0) {
    rail_monitoring();
    rail_counter = 0;
  }

  rail_counter++;
}
