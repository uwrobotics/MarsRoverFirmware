#pragma once

#include <cmath>
#include <iostream>
#include <map>

#include "Logger.h"
#include "Module.h"
#include "PinNames.h"
#include "PinNamesTypes.h"
#include "cmsis.h"
#include "mbed.h"
/*This PDB module is for load, rail and temperature monitoring.*/

class BMSMonitoring final : public Module {
 public:
  /* Sets the Load DIAG_EN pins */
  BMSMonitoring();
  void periodic_1s(void) override;
  void periodic_10s(void) override {}
  void periodic_100ms(void) override {}
  void periodic_10ms(void) override {}
  void periodic_1ms(void) override;

 private:
  void current_monitoring();
  void cell_monitoring();
  void update_voltage_percent();
  void low_battery_warning();
  void send_can_data();

  int m_cell_voltages[12];
  bool m_is_cell_balancing     = false;
  int m_coloumb_count          = 0;
  const int m_battery_capacity = 22000;  // mAh
  int m_battery_percent_couloumb;
  int m_battery_percent_voltage;
  int m_battery_voltage;
  int const m_spi_frequency = 1000000;
  int const m_spi_bits      = 24;
  int const m_spi_mode      = 3;
};