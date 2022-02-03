#pragma once

#include "Module.h"
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
  
  SPI spi;
  DigitalOut chip_select;
  int cell_voltages[12];
  DigitalOut buzzer_en;
  AnalogIn currentADC;
  bool is_cell_balancing = false;
  int coloumb_count = 0;
  const int battery_capacity = 22000; // mAh
  int battery_percent;
};